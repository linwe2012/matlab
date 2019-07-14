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
  Local<Object> js_self_;

  Slider(const FunctionCallbackInfo<Value>& args);
  QHBoxLayout* GetLayout() {
	  return layout_;
  }

  std::vector<QWidget*> GetAllWidget() {
	  return  { this, spinbox_ };
  }

  static void Init(Local<Object> mod, V8Shell* shell);

  Q_OBJECT

private slots:
  void SlideCallback();

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

