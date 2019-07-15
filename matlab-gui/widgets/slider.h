#pragma once
#include "matlab-gui.h"
#include "qslider.h"
#include "qwidget.h"
#include "qlabel.h"
#include "qspinbox.h"
#include "qboxlayout.h"

class Slider : public QSlider {
public:
  //Q_OBJECT
public:
  std::string text_;
  Local<Function> onslide_;
  Local<Function> onrelease_;
  Local<Function> onpress_;
  bool mouse_down_ = false;

  Local<Object> js_self_;

  Slider(const FunctionCallbackInfo<Value>& args);
  QHBoxLayout* GetLayout() {
	  return layout_;
  }

  std::vector<QWidget*> GetAllWidget() {
	  return  { this, spinbox_ };
  }

  void SetValue(int i) {
	  this->setValue(i);
  }

  static void Init(Local<Object> mod, V8Shell* shell);

  Q_OBJECT
	
  

private slots:
  void SlideCallback();
  void Release();
  void Press();

private:
  Isolate* isolate_;
  Local<Context> context_;
  QLabel* name_;
  // QSlider* slider_;
  QDoubleSpinBox* spinbox_;
  QHBoxLayout* layout_;
  static Local<FunctionTemplate> me_class_;
  static v8pp::class_<Slider>* self_class_;
};

