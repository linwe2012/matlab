#include "CheckBox.h"
#include "widget-helper.h"
#include "registry.h"

CheckBox::CheckBox(const FunctionCallbackInfo<Value>& args)
  : QCheckBox(GetTargetWidget())
{

  INIT_OBJECT(args);

  GET(std::string, text);
  GET(Local<Function>, onclick);

  this->setText(text.c_str());
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


void CheckBox::ClickCallback() {
  js_self_ = v8pp::class_<CheckBox>::find_object(isolate_, this);
  Local<Boolean> arg1 = Boolean::New(isolate_, int(this->isChecked()));
  Local<Value> argv[] = { arg1 };
  onclick_ = Local<Function>::Cast(js_self_->Get(MakeStr(isolate_, "onclick_func")));
  onclick_->CallAsFunction(isolate_->GetCurrentContext(), js_self_, 1, argv);
}

Local<FunctionTemplate> CheckBox::me_class_;
v8pp::class_<CheckBox>* CheckBox::self_class_ = nullptr;

void CheckBox::Init(Local<Object> mod, V8Shell* shell)
{
  self_class_ = new
    v8pp::class_<CheckBox>(shell->GetIsolate());
  self_class_
    ->ctor<const FunctionCallbackInfo<Value>&>()
    .set("text", &CheckBox::text_)
    .set("onclick", &CheckBox::onclick_);

  mod->Set(MakeStr(shell->GetIsolate(), "Checkbox"), self_class_->js_function_template()->GetFunction(shell->GetIsolate()->GetCurrentContext()).ToLocalChecked());
}