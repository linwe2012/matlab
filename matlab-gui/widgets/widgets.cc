#include "../matlab-core/matlab.h"
#include "../matlab-core/matlab-experimental.h"

using namespace v8;

class GuiModule {
public:

	
	void RegisterButton(V8Shell* shell);


};
struct Button {
	Button(const FunctionCallbackInfo<Value>& args);

	std::function<void()> click_callback;
};

void ReigsterGui(V8Shell* shell) {
	v8pp::class_<GuiModule> gui(shell->GetIsolate());
	
}

void GuiModule::RegisterButton(V8Shell* shell)
{

}

Local<String> MakeStr(Isolate* i, const char*c) {
	return String::NewFromUtf8(i, c, NewStringType::kNormal).ToLocalChecked();
}

void ArgError(const FunctionCallbackInfo<Value>& args, const char* c) {
	Isolate* isolate = args.GetIsolate();
	isolate->ThrowException({
		Exception::Error(MakeStr(isolate, c))
		});
}

#define GET(type, name) type name =  \
v8pp::from_v8<type>(isolate,         \
obj->Get(String::NewFromUtf8(isolate, #name, NewStringType::kNormal).ToLocalChecked()))

#define INIT_OBJECT(a)                                \
Isolate* isolate = a.GetIsolate();                    \
Local<Context> context = isolate->GetCurrentContext();\
if (a.Length() == 0) {                                \
	ArgError(a, "Expected an object as item");        \
	return;                                           \
}                                                     \
if (!a[0]->IsObject()) {                              \
	ArgError(a, "Expected object");                   \
	return;                                           \
}                                                     \
Local<Object> obj = a[0]->ToObject(context).ToLocalChecked();



Button::Button(const FunctionCallbackInfo<Value>& args)
{
	INIT_OBJECT(args);

	GET(std::string, text);
	GET(Local<Function>, onclick);
	// auto [text, callback] = GetProperties<std::string, Local<Function>>(
	// 	isolate, args[0]->ToObject(context).ToLocalChecked(),
	// 	{"text", "onclick"}
	// 	);

	//add it to list

}
