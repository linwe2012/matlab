#include "slider.h"
#include "widget-helper.h"
#include "registry.h"

Slider::Slider(const FunctionCallbackInfo<Value>& args)
  : QSlider(GetTargetWidget())
{
  INIT_OBJECT(args);

  GET(std::string, text);
  GET(Local<Function>, onslide);
  GET_IF(int, max, 100);
  GET_IF(int, min, 0);
  int nMin = min;
  int nMax = max;

  // QHBoxLayout* layout = new QHBoxLayout(GetTargetWidget());
  layout_ = new QHBoxLayout();
  name_ = new QLabel(text.c_str());
  layout_->setDirection(QHBoxLayout::Up);
  layout_->addWidget(name_);
  //slider_ = new QSlider();
  this->setMinimum(nMin);
  this->setMaximum(nMax);
  //this->setSingleStep(1);
  this->setOrientation(Qt::Horizontal);
  //slider_->setMinimum(nMin);
  //slider_->setMaximum(nMax);
  layout_->addWidget(this);
  spinbox_ = new QDoubleSpinBox();
  spinbox_->setMinimum(nMin);
  spinbox_->setMaximum(nMax);
  layout_->addWidget(spinbox_);
  connect(spinbox_, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));
  connect(this, SIGNAL(valueChanged(int)), spinbox_, SLOT(setValue(int)));
  // this->setValue(0);
  
  
  auto sz = GetCurrentLayout().width();
  sz += this->size().width() + 20;
  GetCurrentLayout().setWidth(sz);

  text_ = text;
  onslide_ = onslide;

  connect(this, SIGNAL(valueChanged(int)), this, SLOT(SlideCallback()));

  isolate_ = isolate;
  context_ = isolate->GetCurrentContext();
  
  js_self_ = args.GetReturnValue().Get()->ToObject(context_).ToLocalChecked();
  js_self_->Set(MakeStr(isolate, "onslide_func"), (onslide));
}

void Slider::SlideCallback() {
	double v = this->value();
  Local<Number> arg1 = Number::New(isolate_, double(this->value()));
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