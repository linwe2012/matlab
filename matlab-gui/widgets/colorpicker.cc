#include "colorpicker.h"
#include "widget-helper.h"
#include "registry.h"

ColorPicker::ColorPicker(const FunctionCallbackInfo<Value>& args)
  : QWidget(GetTargetWidget())
{
  INIT_OBJECT(args);

  GET(Local<Function>, on_color_changed);

  color_.setRgb(255, 255, 255, 255);
  QHBoxLayout* layout = new QHBoxLayout(this);
  button_ = new QPushButton();
  button_->setStyleSheet("\
    border-color:#000000;\
    border-width:2px;\
    border-style:solid;\
    background-color:#FFFFFF;\
    ");
  connect(button_, SIGNAL(clicked()), this, SLOT(ButtonClickedSlot()));
  layout->addWidget(button_);
  slider_ = new QSlider();
  slider_->setMinimum(0);
  slider_->setMaximum(255);
  slider_->setValue(255);
  connect(slider_, SIGNAL(valueChaged(int)), this, SLOT(OpacitySlot(int)));
  layout->addWidget(slider_);

  auto sz = GetCurrentLayout().width();
  sz += this->size().width() + 20;
  GetCurrentLayout().setWidth(sz);
  this->setGeometry(QRect(sz, 530, 220, 22));

  on_color_changed_ = on_color_changed;

  isolate_ = isolate;
  context_ = isolate->GetCurrentContext();

  js_self_ = args.GetReturnValue().Get()->ToObject(context_).ToLocalChecked();
  js_self_->Set(MakeStr(isolate, "on_color_changed_func"), (on_color_changed));
}

void ColorPicker::ButtonClickedSlot()
{
  QColor color = QColorDialog::getColor(color_);
  if (!color.isValid())
    return;
  color_.setRed(color.red());
  color_.setGreen(color.green());
  color_.setBlue(color.blue());
  button_->setStyleSheet("\
      border-color:#000000;\
      border-width:2px;\
      border-style:solid;" +
    QString("background-color:rgb(") +
    QString::number(color.red()) + ',' +
    QString::number(color.green()) + ',' +
    QString::number(color.blue()) + ");");
  ChangedCallback();
}

void ColorPicker::OpacitySlot(int opacity)
{
  color_.setAlpha(opacity);
  ChangedCallback();
}


void ColorPicker::ChangedCallback() 
{
  Local<Number> arg1 = Number::New(isolate_, double(color_.red()));
  Local<Number> arg2 = Number::New(isolate_, double(color_.green()));
  Local<Number> arg3 = Number::New(isolate_, double(color_.blue()));
  Local<Number> arg4 = Number::New(isolate_, double(color_.alpha()));
  Local<Value> argv[] = { arg1,arg2,arg3,arg4 };
  js_self_ = v8pp::class_<ColorPicker>::find_object(isolate_, this);
  on_color_changed_ = Local<Function>::Cast(js_self_->Get(MakeStr(isolate_, "on_color_changed_func")));
  on_color_changed_->CallAsFunction(isolate_->GetCurrentContext(), js_self_, 4, argv);
}

Local<FunctionTemplate> ColorPicker::me_class_;
v8pp::class_<ColorPicker>* ColorPicker::self_class_ = nullptr;

void ColorPicker::Init(Local<Object> mod, V8Shell* shell)
{
  self_class_ = new
    v8pp::class_<ColorPicker>(shell->GetIsolate());
  self_class_
    ->ctor<const FunctionCallbackInfo<Value>&>()
    .set("on_color_changed", &ColorPicker::on_color_changed_);

  mod->Set(MakeStr(shell->GetIsolate(), "ColorPicker"), self_class_->js_function_template()->GetFunction(shell->GetIsolate()->GetCurrentContext()).ToLocalChecked());
}
