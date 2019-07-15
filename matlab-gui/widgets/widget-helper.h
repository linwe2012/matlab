#pragma once
#define GET(type, name) type name =  \
v8pp::from_v8<type>(isolate,         \
obj->Get(String::NewFromUtf8(isolate, #name, NewStringType::kNormal).ToLocalChecked()))

// Get if (type, name fallback) name = fallback if get failed
#define GET_IF(type, name, fallback) type name =  \
v8pp::from_v8<type>(isolate,         \
obj->Get(String::NewFromUtf8(isolate, #name, NewStringType::kNormal).ToLocalChecked()), fallback)



#define GET_V8(name) obj->Get(String::NewFromUtf8(isolate, #name, NewStringType::kNormal).ToLocalChecked())

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

inline Local<String> MakeStr(Isolate* i, const char* c) {
	return String::NewFromUtf8(i, c, NewStringType::kNormal).ToLocalChecked();
}

inline void ArgError(const FunctionCallbackInfo<Value>& args, const char* c) {
	Isolate* isolate = args.GetIsolate();
	isolate->ThrowException({
		Exception::Error(MakeStr(isolate, c))
		});
}

