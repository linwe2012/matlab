#include "shell.h"
#include "libplatform/libplatform.h"

#include <iostream>
#include <assert.h>
#include <filesystem>
namespace fs = std::filesystem;



using namespace v8;
#define v8str(__str__) String::NewFromUtf8(d.context->GetIsolate(), __str__, NewStringType::kNormal).ToLocalChecked()

void DefaultExceptionReporter(std::ostream& os, Isolate* isolate, TryCatch* try_catch);

// Extracts a C string from a V8 Utf8Value.
const char* ToCString(const v8::String::Utf8Value& value) {
	return *value ? *value : "<string conversion failed>";
}

struct V8Shell::Data {
	Data(Isolate* isolate);
	void RegisterShell();

	Isolate* isolate = nullptr;
	
	Local<String> name;
	Local<Context> context;
	std::unique_ptr<Platform> platform;

	bool report_exception = true;
	bool print_result = true;
	bool close = false;

	UncaughtExceptionHandler handler;

	std::ostream* error = &std::cerr;
	std::ostream* output = &std::cerr;

	Isolate::Scope isolate_scope;
	HandleScope handle_scope;
	Context::Scope context_scope;

	V8Shell* super = nullptr;

	std::string cwd;

	fs::path GetPath(fs::path p) {
		return fs::path(cwd) / fs::relative(p, cwd);
	}

	Local<String> ReadFileTxt(const char* name) {
		auto& d = *this;
		std::ifstream ifs(GetPath(name));

		if (!ifs.good() || !ifs.is_open()) {
			isolate->ThrowException(Exception::Error(v8str("Unable to open file")));
			return v8str("");
		}
		
		std::string str((std::istreambuf_iterator<char>(ifs)),
			std::istreambuf_iterator<char>());

		return v8str(str.c_str());
	}

	void ChangeDirectory(const char* str) {
		fs::path path = cwd;
		cwd = fs::weakly_canonical(path / str).string();
	}

	Local<Boolean> FileExist(const char* str) {
		return Boolean::New(isolate, fs::exists(GetPath(str)));
	}
};

V8Shell::Data::Data(Isolate* isolate)
	:   isolate_scope(isolate)
	  , handle_scope(isolate)
	  , context_scope(Context::New(isolate))
{
	this->isolate = isolate;
	this->context = isolate->GetCurrentContext();

}


void V8Shell::Data::RegisterShell()
{
	auto& d = *this;
	v8pp::class_<V8Shell::Data> shell_class(isolate);
	shell_class
		// values
		.set("report_exception", &V8Shell::Data::report_exception)
		.set("print_result", &V8Shell::Data::print_result)
		.set("cwd", &V8Shell::Data::cwd)
		// functions
		.set("readtxt", &V8Shell::Data::ReadFileTxt)
		.set("exists", &V8Shell::Data::FileExist)
		.set("cd", &V8Shell::Data::ChangeDirectory)
		;
	auto shell = v8pp::class_<V8Shell::Data>::import_external(isolate, this);
	auto global = isolate->GetCurrentContext()->Global();
	auto res = global->Set(context, v8str("shell"), shell);

	if (res.IsNothing() || !res.ToChecked()) {
		(*error) << "Shell cannot be loaded" << std::endl;
	}
}

V8Shell::V8Shell(int argc, char* argv[], std::ostream& os, const char* shell_name)
{
	// Initialize V8.
	V8::InitializeICUDefaultLocation(argv[0]);
	V8::InitializeExternalStartupData(argv[0]);
	auto platform = platform::NewDefaultPlatform();
	V8::InitializePlatform(platform.get());
	V8::Initialize();

	// Create a new Isolate and make it the current one.
	Isolate::CreateParams create_params;
	create_params.array_buffer_allocator =
		v8::ArrayBuffer::Allocator::NewDefaultAllocator();

	Isolate* isolate = Isolate::New(create_params);
	data_ = new Data(isolate);
	auto& d = *data_;

	d.handler = [this](Isolate* isolate, TryCatch* try_catch) -> void {
		auto& d = *data_;
		if (d.error) {
			DefaultExceptionReporter(*d.error, isolate, try_catch);
		}
	};

	os << "V8 version " << V8::GetVersion() << " [Javascript shell]" << std::endl;

	d.name = v8str(shell_name);
	d.platform = std::move(platform);
	d.super = this;
	d.cwd = fs::current_path().string();
	d.RegisterShell();
}

bool V8Shell::Execute(const std::string& str, std::function<void(v8::Local<Value>)> callback)
{
	auto& d = *data_;
	
	Context::Scope context_scope(d.context);
	HandleScope handle_scope(d.isolate);
	TryCatch try_catch(d.isolate);
	ScriptOrigin origin(d.name);
	
	Local<String> source = v8str(str.c_str());
	Local<Context> context(d.isolate->GetCurrentContext());
	Local<Script> script;

	if (!v8::Script::Compile(context, source, &origin).ToLocal(&script)) {
		// Print errors that happened during compilation.
		ReportException(&try_catch);
		return false;
	}

	Local<Value> result;
	if (!script->Run(context).ToLocal(&result)) {
		assert(try_catch.HasCaught());
		ReportException(&try_catch);
		return false;
	}
	else {
		assert(!try_catch.HasCaught());
		if (callback) {
			callback(result);
		}

		if (d.print_result && !result->IsUndefined()) {
			String::Utf8Value str(d.isolate, result);
			(*d.output) << ToCString(str) << std::endl;
		}
	}
	return true;
}

void V8Shell::SetUncaughtExceptionHandler(UncaughtExceptionHandler handler)
{
	data_->handler = handler;
}

bool V8Shell::Closed()
{
	return data_->close;
}

v8::Isolate* V8Shell::GetIsolate()
{
	return data_->isolate;
}

void V8Shell::ReportException(TryCatch* try_catch)
{
	auto& d = *data_;
	if (d.report_exception && d.handler) {
		d.handler(d.isolate, try_catch);
	}
}


void DefaultExceptionReporter(std::ostream& os, Isolate* isolate, TryCatch* try_catch) {
	HandleScope handle_scope(isolate);
	String::Utf8Value exception(isolate, try_catch->Exception());

	const char* exception_string = ToCString(exception);
	Local<Message> message = try_catch->Message();

	V8Exception parsed(isolate, try_catch);
	if (!parsed.has_detail) {
		os << parsed.exception << std::endl;
	}
	else {
		os << parsed.filename << ":"
			<< parsed.line_number << ": "
			<< parsed.exception << std::endl;

		os << parsed.sourceline << std::endl;

		for (int i = 0; i < parsed.sourceline_begin; i++) {
			os << " ";
		}
		for (int i = parsed.sourceline_begin; i < parsed.sourceline_end; i++) {
			os << "^";
		}
		os << std::endl;

		if (parsed.stack_trace.size()) {
			os << "[stack]: " << parsed.stack_trace << std::endl;
		}
	}
}

V8Exception::V8Exception(Isolate* isolate, TryCatch* try_catch)
{
	HandleScope handle_scope(isolate);
	String::Utf8Value v8exception(isolate, try_catch->Exception());
	this->exception = ToCString(v8exception);
	Local<Message> message = try_catch->Message();

	// V8 didn't provide any extra information about this error
	if (message.IsEmpty()) {
		return;
	}


	this->has_detail = true;
	String::Utf8Value v8filename(isolate,
		message->GetScriptOrigin().ResourceName());

	Local<Context> context(isolate->GetCurrentContext());
	this->filename = ToCString(v8filename);

	this->line_number = message->GetLineNumber(context).FromJust();

	String::Utf8Value v8sourceline(
		isolate, message->GetSourceLine(context).ToLocalChecked());
	this->sourceline = ToCString(v8sourceline);


	this->sourceline_begin = message->GetStartColumn(context).FromJust();
	this->sourceline_end = message->GetEndColumn(context).FromJust();

	Local<Value> stack_trace_string;
	if (try_catch->StackTrace(context).ToLocal(&stack_trace_string) &&
		stack_trace_string->IsString() &&
		Local<String>::Cast(stack_trace_string)->Length() > 0) {
		String::Utf8Value v8stack_trace(isolate, stack_trace_string);
		this->stack_trace = ToCString(v8stack_trace);
	}
}


