#pragma once
#include "matlab-gui.h"
#include "qpushbutton.h"

class Slider : public QSlider{
public:
  //Q_OBJECT
public:
  std::string text_;
  Local<Function> onclick_;
  Local<Object> js_self_;

  Slider(const FunctionCallbackInfo<Value>& args);

  static void Init(Local<Object> mod, V8Shell* shell);

  Q_OBJECT

private slots:
  void SlideCallback();

private:
  Isolate* isolate_;
  Local<Context> context_;
  static Local<FunctionTemplate> me_class_;
  static v8pp::class_<Slider>* self_class_;
};

