#include "button.h"
#include "widget-helper.h"
#include "registry.h"

Button::Button(const FunctionCallbackInfo<Value>& args)
	: QToolButton(GetTargetWidget())
{
	// auto [text, callback] = GetProperties<std::string, Local<Function>>(
	// 	isolate, args[0]->ToObject(context).ToLocalChecked(),
	// 	{"text", "onclick"}
	// 	);

	INIT_OBJECT(args);

	GET(std::string, text);
	GET(Local<Function>, onclick);
	GET_IF(std::string, icon, "");

	if (!icon.empty()) {
		this->setIcon(QIcon(icon.c_str()));
	}
	this->setText(text.c_str());
	// this->resize(QSize(900, 1000));
	auto sz = GetCurrentLayout().width();
	sz += this->size().width() + 20;
	GetCurrentLayout().setWidth(sz);
	this->setGeometry(QRect(sz, 530, 160, 22));

	text_ = text;
	onclick_ = onclick;


	connect(this, SIGNAL(clicked(bool)), this, SLOT(ClickCallback()));

	isolate_ = isolate;
	context_ = isolate->GetCurrentContext();

	js_self_ = args.GetReturnValue().Get()->ToObject(context_).ToLocalChecked();
	js_self_->Set(MakeStr(isolate, "onclick_func"), (onclick));
}


void Button::ClickCallback() {
	js_self_ = v8pp::class_<Button>::find_object(isolate_, this);
	//auto context = isolate_->GetCurrentContext();
	//Local<Array> property_names = js_self_->GetOwnPropertyNames(context).ToLocalChecked();
	//int cnt = property_names->Length();
	//for (int i = 0; i < cnt; ++i) {
	//	Local<Value> key = property_names->Get(i);
	//	Local<Value> value = js_self_->Get(context, key).ToLocalChecked();
	//
	//	if (key->IsString()) {
	//		auto v8_str = key->ToString(context).ToLocalChecked();
	//		String::Utf8Value utf8(isolate_, v8_str);
	//	}
	//	else {
	//		// Throw an error or something
	//	}
	//}
	onclick_ = Local<Function>::Cast(js_self_->Get(MakeStr(isolate_, "onclick_func")));
	onclick_->CallAsFunction(isolate_->GetCurrentContext(), js_self_, 0, nullptr);
}

Local<FunctionTemplate> Button::me_class_;
v8pp::class_<Button>* Button::self_class_ = nullptr;

void Button::Init(Local<Object> mod, V8Shell* shell)
{
	self_class_ = new
		v8pp::class_<Button>(shell->GetIsolate());
	self_class_
		->ctor<const FunctionCallbackInfo<Value>&>()
		.set("text", &Button::text_)
		.set("onclick", &Button::onclick_);

	mod->Set(MakeStr(shell->GetIsolate(), "Button"), self_class_->js_function_template()->GetFunction(shell->GetIsolate()->GetCurrentContext()).ToLocalChecked());
}