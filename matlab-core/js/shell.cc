#include "shell.h"
#include "libplatform/libplatform.h"

#include <iostream>
#include <assert.h>
#include <filesystem>
namespace fs = std::filesystem;

using namespace v8;

//   U T I L S
//-----------------------------------
#define v8str(__str__) String::NewFromUtf8(d.context->GetIsolate(), __str__, NewStringType::kNormal).ToLocalChecked()

void DefaultExceptionReporter(std::ostream& os, Isolate* isolate, TryCatch* try_catch);

// Extracts a C string from a V8 Utf8Value.
const char* ToCString(const v8::String::Utf8Value& value) {
	return *value ? *value : "<string conversion failed>";
}

void ReloadInitFile(V8Shell* shell);

//   D A T A   &   I T 'S    F R I E N D S
//--------------------------------------------
struct V8Shell::Data {
	
	static std::map<Isolate*, V8Shell*> shell_map;
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

	std::vector<std::string> cwd_stack;

	Local<Object> shell_object;

	std::vector<Local<Context>> context_stack;

	Local<ObjectTemplate> global_object_template;


	// functions exposed to v8
	//--------------------------
	fs::path GetPath(fs::path p) {
		return fs::path(cwd) / fs::relative(p, cwd);
	}

	Local<String> ReadFileTxt(const char* name);

	void ChangeDirectory(const char* str);

	Local<Boolean> FileExist(const char* str);
	
	void CreateGlobal();

	void SetExit() { close = true; }

	void PushCwd() { cwd_stack.push_back(cwd); }

	void PopCwd(Isolate* isolate) {
		auto& d = *this;
		if (cwd_stack.size() == 0) {
			isolate->ThrowException(
				Exception::ReferenceError(v8str("Stack is empty"))
			);
			return;
		}

		cwd = cwd_stack.back();
		cwd_stack.pop_back();
	}

	// returns arrays of file names
	void IterateFiles(const FunctionCallbackInfo<Value>& vals);

	std::string GetDirectory(const char* f) {
		return GetPath(f).parent_path().string();
	}

	std::string Extension(const char* f) {
		return fs::path(f).extension().string();
	}
};
std::map<Isolate*, V8Shell*> V8Shell::Data::shell_map;

V8Shell::Data::Data(Isolate* isolate)
	:   isolate_scope(isolate)
	  , handle_scope(isolate)
	  , context_scope(Context::New(isolate))
{
	this->isolate = isolate;
	this->context = isolate->GetCurrentContext();

}


void V8Shell::Data::CreateGlobal() {
	global_object_template = v8::ObjectTemplate::New(isolate);

	auto& d = *this;
	
	context_stack.push_back(
		Context::New(isolate, nullptr, global_object_template)
	);

	context_stack.back()->Enter();
}

void V8Shell::Data::IterateFiles(const FunctionCallbackInfo<Value>& args)
{
	Local<Array> arr = Array::New(isolate);
	auto& d = *this;
	std::vector<std::string> paths;
	auto lmbdAdd = [&](Local<Value> v) {
		paths.push_back(
			std::move(
				v8pp::from_v8<std::string>(isolate, v)
			)
		);
	};

	int len = args.Length();

	if (args.Length() == 0) {
		paths.push_back("./");
	}
	else {
		if (args[0]->IsString()) {
			for (int i = 0; i < len; ++i) {
				lmbdAdd(args[i]);
			}
		}
		else if(args[0]->IsArray()){
			
			paths = std::move(
				v8pp::from_v8<std::vector<std::string>>(isolate, args[0])
			);
		}
		else {
			isolate->ThrowException(
				Exception::ReferenceError(
					v8str("Expected string or array of strings")
				)
			);
			args.GetReturnValue().Set(arr);
			return;
		}
	}

	
	uint32_t cnt = 0;
	std::vector<std::string> native;
	for (auto& path : paths) {
		if (!fs::exists(path)) {
			//TODO: Log warning
			continue;
		}

		if (!fs::is_directory(path)) {
			//TODO: Log warning
			continue;
		}

		for (auto file : fs::directory_iterator(path)) {
			arr->Set(d.isolate->GetCurrentContext(), cnt, v8str(file.path().string().c_str()));
			++cnt;
		}
	}
	args.GetReturnValue().Set(arr);
	return;
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
		.set("exit", &V8Shell::Data::SetExit)
		.set("listfiles", &V8Shell::Data::IterateFiles)
		.set("pushCwd", &V8Shell::Data::PushCwd)
		.set("popCwd", &V8Shell::Data::PopCwd)
		.set("getDir", &V8Shell::Data::GetDirectory)
		.set("getExt", &V8Shell::Data::Extension)
		;
	auto shell = v8pp::class_<V8Shell::Data>::reference_external(isolate, this);
	//auto global = isolate->GetCurrentContext()->Global();
	//auto res = global->Set(context, v8str("shell"), shell);
	
	
	shell_object = shell;
	CreateGlobal();
	auto global = isolate->GetCurrentContext()->Global();
	auto res = global->Set(context_stack.back(), v8str("shell"), shell);

	if (res.IsNothing() || !res.ToChecked()) {
		(*error) << "Shell cannot be loaded" << std::endl;
	}

}


Local<String> V8Shell::Data::ReadFileTxt(const char* name) {
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


void V8Shell::Data::ChangeDirectory(const char* str) {
	fs::path path = cwd;
	cwd = fs::weakly_canonical(path / str).string();
}


Local<Boolean> V8Shell::Data::FileExist(const char* str) {
	return Boolean::New(isolate, fs::exists(GetPath(str)));
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
	ReloadInitFile(this);

	d.shell_map.insert({ d.isolate, this });
}


bool V8Shell::Execute(const std::string& str, std::function<void(v8::Local<Value>)> callback)
{
	auto& d = *data_;
	
	Context::Scope context_scope(d.context_stack.back());
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


void V8Shell::RegisterClasses(const std::vector<Class>& classes)
{
	auto& d = *data_;
	
	for (auto c : classes) {
		d.context_stack.back()->Global()->Set(
			v8str(c.name.c_str()), c.ctor->GetFunction(d.context_stack.back()).ToLocalChecked()
		);
	}
}

void V8Shell::RegisterGlobals(const std::vector<Global>& globals)
{
	auto& d = *data_;
	for (auto g : globals) {
		d.context_stack.back()->Global()->Set(
			v8str(g.name.c_str()), g.obj
		);
	}
}

std::string V8Shell::GetCwd()
{
	return data_->cwd;
}

V8Shell* V8Shell::GetShell(v8::Isolate* i)
{
	auto itr = Data::shell_map.find(i);
	if (itr == Data::shell_map.end()) {
		return nullptr;
	}
	return itr->second;
}

V8Shell::~V8Shell()
{
	auto& d = *data_;

	
	while (d.context_stack.size()) {
		d.context_stack.back()->Exit();
		d.context_stack.pop_back();
	}
	v8pp::class_<V8Shell::Data>::unreference_external(d.isolate, data_);
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

void ReloadInitFile(V8Shell* shell)
{
	if (fs::exists("__init__.js")) {
		std::ifstream ifs("__init__.js");

		std::string str = std::string(
			std::istreambuf_iterator<char>(ifs),
			std::istreambuf_iterator<char>()
		);

		shell->Execute(str);
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


