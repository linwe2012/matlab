#pragma once
#include "matlab-gui.h"
#include "qcheckbox.h"
class CheckBox : public QCheckBox
{
public:
  std::string text_;
  Local<Function> onclick_;
  Local<Object> js_self_;

  CheckBox(const FunctionCallbackInfo<Value>& args);

  static void Init(Local<Object> mod, V8Shell* shell);

  Q_OBJECT

private slots:
  void ClickCallback();

private:
  Isolate* isolate_;
  Local<Context> context_;
  static Local<FunctionTemplate> me_class_;
  static v8pp::class_<CheckBox>* self_class_;
};
