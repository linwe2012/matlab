#pragma once
#include "v8.h"
class JSVisitor {
public:
	template<typename T>
	using Local = v8::Local<T>;

	virtual void Visit(nullptr_t) = 0;

	virtual void Visit(Local<v8::String>) = 0;

	virtual void Visit(Local<v8::StringObject>) = 0;

	virtual void Visit(Local<v8::BigInt>) = 0;

	virtual void Visit(Local<v8::BigIntObject>) = 0;

	virtual void Visit(Local<v8::Symbol>) = 0;

	virtual void Visit(Local<v8::SymbolObject>) = 0;

	virtual void Visit(Local<v8::NumberObject>) = 0;
	
	virtual void Visit(Local<v8::Boolean>) = 0;

	virtual void Visit(Local<v8::BooleanObject>) = 0;

	virtual void Visit(Local<v8::Array>) = 0;

	virtual void Visit(Local<v8::Proxy>) = 0;

	virtual void Visit(Local<v8::Object>) = 0;

	virtual void Visit(Local<v8::Function>) = 0;

	virtual void Visit(Local<v8::Date>) = 0;

	virtual void Visit(Local<v8::RegExp>) = 0;

	void Dispatch(v8::Isolate* isolate, Local<v8::Value>);
};


