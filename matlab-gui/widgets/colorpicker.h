#pragma once
#include "matlab-gui.h"
#include "qwidget.h"
#include "qcolordialog.h"
#include "qpushbutton.h"
#include "qboxlayout.h" 
#include "qstring.h"

class ColorPicker : public QWidget {
public:
  //Q_OBJECT
public:
  std::string text_;
  Local<Function> on_color_changed_;
  Local<Object> js_self_;

  ColorPicker(const FunctionCallbackInfo<Value>& args);

  static void Init(Local<Object> mod, V8Shell* shell);

  Q_OBJECT

private slots:
  void ButtonClickedSlot();
  void OpacitySlot(int);
  void ChangedCallback();
  
private:
  Isolate* isolate_;
  Local<Context> context_;
  QColor color_;
  QPushButton* button_;
  QSlider* slider_;
  static Local<FunctionTemplate> me_class_;
  static v8pp::class_<ColorPicker>* self_class_;
};

