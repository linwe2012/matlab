#include "slider.h"
#include "widget-helper.h"
#include "registry.h"

Slider::Slider(const FunctionCallbackInfo<Value>& args)
  : QWidget(GetTargetWidget())
{
  INIT_OBJECT(args);

  GET(std::string, text);
  GET(Local<Function>, onslide);
  int nMin = 0;
  int nMax = 100;

  QHBoxLayout* layout = new QHBoxLayout(this);
  name_ = new QLabel(text.c_str());
  layout->addWidget(name_);
  slider_ = new QSlider();
  slider_->setMinimum(nMin);
  slider_->setMaximum(nMax);
  layout->addWidget(slider_);
  spinbox_ = new QDoubleSpinBox();
  spinbox_->setMinimum(nMin);
  spinbox_->setMaximum(nMax);
  layout->addWidget(spinbox_);
  connect(spinbox_, SIGNAL(valueChanged(int)), slider_, SLOT(setValue(int)));
  connect(slider_, SIGNAL(valueChanged(int)), spinbox_, SLOT(setValue(int)));
  slider_->setValue(0);

  auto sz = GetCurrentLayout().width();
  sz += this->size().width() + 20;
  GetCurrentLayout().setWidth(sz);
  this->setGeometry(QRect(sz, 530, 200, 22));

  text_ = text;
  onslide_ = onslide;

  connect(slider_, SIGNAL(valueChanged(int)), this, SLOT(SlideCallback()));

  isolate_ = isolate;
  context_ = isolate->GetCurrentContext();

  js_self_ = args.GetReturnValue().Get()->ToObject(context_).ToLocalChecked();
  js_self_->Set(MakeStr(isolate, "onslide_func"), (onslide));
}

void Slider::SlideCallback() {
  Local<Number> arg1 = Number::New(isolate_, double(spinbox_->value()));
  Local<Value> argv[] = { arg1 };
  js_self_ = v8pp::class_<Slider>::find_object(isolate_, this);
  onslide_ = Local<Function>::Cast(js_self_->Get(MakeStr(isolate_, "onslide_func")));
  onslide_->CallAsFunction(isolate_->GetCurrentContext(), js_self_, 1, argv);
}

Local<FunctionTemplate> Slider::me_class_;
v8pp::class_<Slider>* Slider::self_class_ = nullptr;

void Slider::Init(Local<Object> mod, V8Shell* shell)
{
  self_class_ = new
    v8pp::class_<Slider>(shell->GetIsolate());
  self_class_
    ->ctor<const FunctionCallbackInfo<Value>&>()
    .set("text", &Slider::text_)
    .set("onslide", &Slider::onslide_);


  mod->Set(MakeStr(shell->GetIsolate(), "Slider"), self_class_->js_function_template()->GetFunction(shell->GetIsolate()->GetCurrentContext()).ToLocalChecked());
}