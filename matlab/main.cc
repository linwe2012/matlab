#include <functional>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <assert.h>

#include "v8.h"
#include "libplatform/libplatform.h"

class Model {
	std::string display_str_;
};

class Message {
public:
	Message(std::string command) : command_(command) {}
	const std::string& command() { return command_; }
	Message() {};

private:
	std::string command_;
	// std::shared_ptr<Model> target_;
};



class ViewModel {
public:

	void OnClick(Message msg) {
		std::cout << msg.command() << std::endl;
	}

	std::shared_ptr<Model> target_;
};


class Window {
public:
	using Callback = std::function<void(Message)>;
	void Register(std::string str, Callback f) {
		callbacks_[str] = f;
	}

	void Update() {
		Message msg;
		for (auto& cb : callbacks_) {
			cb.second(msg);
		}
	}

	void Fire(std::string str, Message msg = Message{ std::string("Hello World") }) {
		auto it = callbacks_.find(str);
		if (it != callbacks_.end()) {
			it->second(msg);
		}
	}

private:
	std::map<std::string, Callback> callbacks_;
};

int main(int argc, char* argv[])
{
	Window window;
	ViewModel view_model;

	window.Register("Button.Click", [&](Message msg) {
		view_model.OnClick(msg);
	});

	window.Fire("Button.Click");
	getchar();

	using namespace v8;

	// Initialize V8.
	V8::InitializeICUDefaultLocation(argv[0]);
	V8::InitializeExternalStartupData(argv[0]);
	auto platform = platform::NewDefaultPlatform();
	v8::V8::InitializePlatform(platform.get());
	v8::V8::Initialize();

	// Create a new Isolate and make it the current one.
	Isolate::CreateParams create_params;
	create_params.array_buffer_allocator =
		v8::ArrayBuffer::Allocator::NewDefaultAllocator();

	Isolate* isolate = v8::Isolate::New(create_params);
	{
		Isolate::Scope isolate_scope(isolate);
		HandleScope handle_scope(isolate);

		// Enter the execution environment before evaluating any code.
		auto context = v8::Context::New(isolate);
		Context::Scope context_scope(context);

		fprintf(stderr, "V8 version %s [Javascript shell]\n", V8::GetVersion());
		static const int kBufferSize = 256;
		Local<String> name(
			String::NewFromUtf8(context->GetIsolate(), "(shell)",
				NewStringType::kNormal).ToLocalChecked());

		while (true) {
			char buffer[kBufferSize];
			fprintf(stderr, "> ");
			char* str = fgets(buffer, kBufferSize, stdin);
			if (str == NULL) break;
			v8::HandleScope handle_scope(context->GetIsolate());

			TryCatch try_catch(isolate);
			ScriptOrigin origin(name);
			Local<String> source = String::NewFromUtf8(context->GetIsolate(), str,
				v8::NewStringType::kNormal).ToLocalChecked();

			v8::Local<v8::Context> context(isolate->GetCurrentContext());
			v8::Local<v8::Script> script;
			if (!v8::Script::Compile(context, source, &origin).ToLocal(&script)) {
				fprintf(stderr, "(Failed to compile)\n");
			}
			else {
				Local<v8::Value> result;
				if (!script->Run(context).ToLocal(&result)) {
					assert(try_catch.HasCaught());
					fprintf(stderr, "(exception caught)\n");
				}
				else {
					if (!result->IsUndefined()) {
						String::Utf8Value str(isolate, result);
						printf("%s\n", *str ? *str : "(unable to stringify)");
					}
				}
			}
		}
	}
	


	return 0;
}