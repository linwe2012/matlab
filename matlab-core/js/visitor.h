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

	virtual void Visit(Local<v8::Number>) = 0;

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

class SimpleJSVisitor {
public:
	template<typename T>
	using Local = v8::Local<T>;

	virtual void Visit(nullptr_t) = 0;

	virtual void Visit(const char*) = 0;

	virtual void Visit(Local<v8::BigInt>) = 0;

	virtual void Visit(double) = 0;

	virtual void Visit(bool) = 0;

	virtual void Visit(Local<v8::Array>) = 0;

	virtual void Visit(Local<v8::Object>) = 0;
};

class SimpleJSVisitorDispatcher : public JSVisitor {
public:
	template<typename T>
	using Local = v8::Local<T>;

	void Visit(nullptr_t n) override { target->Visit(n); }

	void Visit(Local<v8::String> v) override { 
		String::Utf8Value utf8(isolate, v);
		target->Visit(*utf8);
	};

	void Visit(Local<v8::StringObject> v) override {
		Visit(v->ValueOf());
	}

	void Visit(Local<v8::BigInt> v) override {
		target->Visit(v);
	}

	void Visit(Local<v8::BigIntObject> v) {
		Visit(v->ValueOf());
	}

	void Visit(Local<v8::Symbol> v) override {
		if (v->Name().IsEmpty()) {
			target->Visit(nullptr);
		}
		else {
			Dispatch(isolate, v->Name());
		}
	}

	void Visit(Local<v8::SymbolObject> v) {
		Visit(v->ValueOf());
	}

	void Visit(Local<v8::Number> v) override {
		target->Visit(v->Value());
	}

	void Visit(Local<v8::NumberObject> v) override {
		target->Visit(v->ValueOf());
	}

	void Visit(Local<v8::Boolean> v) override {
		target->Visit(v->Value());
	}

	void Visit(Local<v8::BooleanObject> v) override {
		target->Visit(v->ValueOf());
	}

	void Visit(Local<v8::Array> v) override {
		target->Visit(v);
	}

	void Visit(Local<v8::Proxy>) override {
		//TODO
	}

	void Visit(Local<v8::Object> v) override {
		target->Visit(v);
	}

	void Visit(Local<v8::Function>) override {

	}

	void Visit(Local<v8::Date>) override {
		//TODO
	}

	void Visit(Local<v8::RegExp>) override {
		//TODO
	}

	void Bind(SimpleJSVisitor* _targte, Isolate* _isolate) {
		target = _targte;
		isolate = _isolate;
	}

	SimpleJSVisitor* target;
	Isolate* isolate;
};



