#include "../matlab-core/matlab.h"
#include "../matlab-core/matlab-experimental.h"
#include "matlab-gui.h"
#include "qpushbutton.h"
#include "button.h"

using namespace v8;

Local<String> MakeStr(Isolate* i, const char* c) {
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

void ReigsterGui(V8Shell* shell);


QWidget* target_widgets;

int global_x = 0;
Button::Button(const FunctionCallbackInfo<Value>& args)
	: QPushButton(target_widgets)
{
	// auto [text, callback] = GetProperties<std::string, Local<Function>>(
	// 	isolate, args[0]->ToObject(context).ToLocalChecked(),
	// 	{"text", "onclick"}
	// 	);

	INIT_OBJECT(args);

	GET(std::string, text);
	GET(Local<Function>, onclick);
	this->setText(text.c_str());
	this->resize(QSize(900, 1000));
	this->setGeometry(QRect(570 + global_x, 530, 160, 22));
	global_x += 100;

	text_ = text;
	onclick_ = onclick;
	

	connect(this, SIGNAL(clicked(bool)), this, SLOT(ClickCallback()));
	
	
	// auto& self = *js_class_;
	isolate_ = isolate;
	context_ = isolate->GetCurrentContext();
	
	js_self_ = args.GetReturnValue().Get()->ToObject(context_).ToLocalChecked();
	// js_self_->Set(MakeStr(isolate, "text"), GET_V8(text));
	js_self_->Set(MakeStr(isolate, "onclick_func"), (onclick));
	
	//js_self_->Set(MakeStr(isolate, "text"), GET_V8(text));
	//js_self_->Set(MakeStr(isolate, "onclick"), GET_V8(onclick));
	// onclick->CallAsFunction(context_, obj, 0, nullptr);


	// show();
	//js_self_ = self.import_external(isolate, this);
	//args.GetReturnValue().Set(js_self_);
}

Local<FunctionTemplate> Button::me_class_;
v8pp::class_<Button>* Button::self_class_ = nullptr;

void Button::New(const FunctionCallbackInfo<Value>& args)
{
	
	auto self = new Button();
	INIT_OBJECT(args);
	

	GET(Local<Object>, text_js);
	GET(std::string, text);
	GET(Local<Function>, onclick);
	self->setText(text.c_str());
	self->resize(QSize(900, 1000));
	self->setGeometry(QRect(570 + global_x, 530, 160, 22));
	global_x += 100;

	self->text_ = text;
	self->onclick_ = onclick;

	connect(self, SIGNAL(clicked(bool)), self, SLOT(ClickCallback()));

	auto& js_self = self->js_self_;

	self->isolate_ = isolate;
	self->context_ = isolate->GetCurrentContext();

	js_self = me_class_->GetFunction(isolate->GetCurrentContext()).ToLocalChecked()->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();

	js_self->SetInternalField(0, External::New(args.GetIsolate(), self));
	js_self->Set(MakeStr(isolate, "text"), GET_V8(text));
	js_self->Set(MakeStr(isolate, "onclick"), GET_V8(onclick));

	self->onclick_ = Local<Function>::Cast(self->js_self_->Get(isolate->GetCurrentContext(), MakeStr(isolate, "onclick")).ToLocalChecked());
	
	args.GetReturnValue().Set(js_self);
}

void Button::Init(Local<Object> mod, V8Shell* shell)
{
	self_class_ = new
		v8pp::class_<Button>(shell->GetIsolate());
	self_class_
		->ctor<const FunctionCallbackInfo<Value>&>()
		.set("text", &Button::text_)
		.set("onclick", &Button::onclick_);
	
	// nTemplate::New(shell->GetIsolate(), Button::New);
	//me_class_->InstanceTemplate()->SetInternalFieldCount(1);
	mod->Set(MakeStr(shell->GetIsolate(), "Button"), self_class_->js_function_template()->GetFunction(shell->GetIsolate()->GetCurrentContext()).ToLocalChecked());
}

Button::Button()
	: QPushButton(target_widgets)
{
}

void Button::ClickCallback() {
	// auto val = js_self_->Get(isolate_->GetCurrentContext(), MakeStr(isolate_, "onclick")).ToLocalChecked();
	// auto obj = val->ToObject(isolate_->GetCurrentContext()).ToLocalChecked();
	// obj->CallAsFunction(isolate_->GetCurrentContext(), js_self_, 0, nullptr);

	js_self_ = v8pp::class_<Button>::find_object(isolate_, this);
	auto context = isolate_->GetCurrentContext();
	Local<Array> property_names = js_self_->GetOwnPropertyNames(context).ToLocalChecked();
	int cnt = property_names->Length();
	for (int i = 0; i < cnt; ++i) {
		Local<Value> key = property_names->Get(i);
		Local<Value> value = js_self_->Get(context, key).ToLocalChecked();

		if (key->IsString()) {
			auto v8_str = key->ToString(context).ToLocalChecked();
			String::Utf8Value utf8(isolate_, v8_str);
		}
		else {
			// Throw an error or something
		}
	}
	onclick_ = Local<Function>::Cast(js_self_->Get(MakeStr(isolate_, "onclick_func")));
	onclick_->CallAsFunction(isolate_->GetCurrentContext(), js_self_, 0, nullptr);
}

struct GuiModule {
	void Display(const FunctionCallbackInfo<Value>& args) {
		Matrix *mat = 
		v8pp::class_<Matrix>::unwrap_object(args.GetIsolate(), args[0]);

		auto& img = mat->matrix;
		image_scene->addPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));
		image_view->show();
	}

	QGraphicsView* image_view;
	QGraphicsScene* image_scene;
};

void ReigsterGui(V8Shell* shell, QMainWindow* main)
{
	v8pp::class_<GuiModule> gui(shell->GetIsolate());
	gui
		.set("display", &GuiModule::Display)
		;
	Local<Object> mod = gui.create_object(shell->GetIsolate());

	Button::Init(mod, shell);
	GuiModule& pmod = *gui.unwrap_object(shell->GetIsolate(), mod);

	
	
	shell->RegisterGlobals(
		{
			{"gui", mod}
		}
	);

	pmod.image_view = new QGraphicsView(target_widgets);
	pmod.image_view->setObjectName(QString::fromUtf8("image_view"));
	pmod.image_view->setGeometry(QRect(20, 50, 421, 321));

	pmod.image_scene = new QGraphicsScene(main);

	pmod.image_view->setScene(pmod.image_scene);
	

	target_widgets = new QWidget(main);
	target_widgets->setObjectName(QString::fromUtf8("toolbar"));
	main->setCentralWidget(target_widgets);
	// QMetaObject::connectSlotsByName(main);
}


