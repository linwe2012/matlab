#pragma once
#include "qtoolbutton.h"
#include "matlab-gui.h"

class DragTool : public QToolButton
{
public:
  std::string text_;
  Local<Function> onpress_;
  Local<Function> onmove_;
  Local<Function> onrelease_;
  Local<Object> js_self_;

  DragTool(const FunctionCallbackInfo<Value>& args);

  static void Init(Local<Object> mod, V8Shell* shell);
  bool eventFilter(QObject* target, QEvent* event);

  Q_OBJECT

private slots:
  void ClickSlot(bool);

private:
  Isolate* isolate_;
  Local<Context> context_;
  static Local<FunctionTemplate> me_class_;
  static v8pp::class_<DragTool>* self_class_;
  static DragTool* active_;
};

