#pragma once
#include "qtextedit.h"
#include "matlab-gui.h"
#include "qboxlayout.h"

class TextEdit : public QTextEdit
{
public:
  //Q_OBJECT
public:
  std::string text_;
  Local<Function> onenter_;
  Local<Object> js_self_;

  TextEdit(const FunctionCallbackInfo<Value>& args);

  static void Init(Local<Object> mod, V8Shell* shell);

  Q_OBJECT

private slots:
  void EnterCallback();
  void CursorPositionChangeSlot();
  bool eventFilter(QObject* obj, QEvent* event);

private:
  Isolate* isolate_;
  Local<Context> context_;
  static Local<FunctionTemplate> me_class_;
  static v8pp::class_<TextEdit>* self_class_;
  int cmd_start_pos_;
  bool write_enable_;
  bool cursor_at_cmd_head_;
};


