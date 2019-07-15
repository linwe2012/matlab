#include "slider.h"
#include "widget-helper.h"
#include "registry.h"

Slider::Slider(const FunctionCallbackInfo<Value>& args)
  : QSlider(GetTargetWidget())
{
  INIT_OBJECT(args);

  GET(std::string, text);
  GET(Local<Function>, onslide);
  GET_IF(Local<Function>, onpress, Local<Function>());
  GET_IF(Local<Function>, onrelease, Local<Function>());
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
  onpress_ = onpress;
  onrelease_ = onrelease;

  connect(this, SIGNAL(sliderMoved(int)), this, SLOT(SlideCallback()));
  connect(this, SIGNAL(sliderReleased()), this, SLOT(Release()));
  connect(this, SIGNAL(sliderPressed()), this, SLOT(Press()));
  isolate_ = isolate;
  context_ = isolate->GetCurrentContext();
  
  js_self_ = args.GetReturnValue().Get()->ToObject(context_).ToLocalChecked();
  js_self_->Set(MakeStr(isolate, "onslide_func"), (onslide));
  if (!onrelease_.IsEmpty()) {
	  js_self_->Set(MakeStr(isolate, "onrelease_func"), (onrelease_));
  }
  if (!onpress_.IsEmpty()) {
	  js_self_->Set(MakeStr(isolate, "onpress_func"), (onpress_));
  }
}

void Slider::SlideCallback() {
	double v = this->value();
  Local<Number> arg1 = Number::New(isolate_, double(this->value()));
  Local<Value> argv[] = { arg1 };
  js_self_ = v8pp::class_<Slider>::find_object(isolate_, this);
  onslide_ = Local<Function>::Cast(js_self_->Get(MakeStr(isolate_, "onslide_func")));
  onslide_->CallAsFunction(isolate_->GetCurrentContext(), js_self_, 1, argv);
}


void Slider::Press()
{
	js_self_ = v8pp::class_<Slider>::find_object(isolate_, this);
	auto l = js_self_->Get(MakeStr(isolate_, "onpress_func"));
	if (l.IsEmpty() || l->IsNullOrUndefined()) {
		return;
	}
	onrelease_ = Local<Function>::Cast(l);
	onrelease_->CallAsFunction(isolate_->GetCurrentContext(), js_self_, 0, nullptr);
}

void Slider::Release()
{
	auto l = js_self_->Get(MakeStr(isolate_, "onrelease_func"));
	if (l.IsEmpty() || l->IsNullOrUndefined()) {
		return;
	}
	onrelease_ = Local<Function>::Cast(l);
	onrelease_->CallAsFunction(isolate_->GetCurrentContext(), js_self_, 0, nullptr);
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
	  .set("onslide", &Slider::onslide_)
	  .set("setValue", &Slider::SetValue)
	  ;

  mod->Set(MakeStr(shell->GetIsolate(), "Slider"), self_class_->js_function_template()->GetFunction(shell->GetIsolate()->GetCurrentContext()).ToLocalChecked());
}