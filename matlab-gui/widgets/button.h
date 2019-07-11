#pragma once
#include "matlab-gui.h"
#include "qpushbutton.h"

class Button : public QPushButton {
public:
	//Q_OBJECT
public:
	std::string text_;
	Local<Function> onclick_;
	Local<Object> js_self_;

	Button(const FunctionCallbackInfo<Value>& args);
	
	static void New(const FunctionCallbackInfo<Value>& args);

	static void Init(Local<Object> mod, V8Shell* shell);

	Button();

	Q_OBJECT

private slots:
	void ClickCallback();

private:
	Isolate* isolate_;
	Local<Context> context_;
	static Local<FunctionTemplate> me_class_;
	static v8pp::class_<Button>* self_class_;
};