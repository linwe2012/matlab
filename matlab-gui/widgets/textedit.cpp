#include "textedit.h"
#include "widget-helper.h"
#include "registry.h"

TextEdit::TextEdit(const FunctionCallbackInfo<Value>& args)
  : QTextEdit(GetTargetWidget())
{

  INIT_OBJECT(args);

  GET(std::string, text);
  GET(Local<Function>, onenter);

  this->setText(text.c_str());
  auto sz = GetCurrentLayout().width();
  sz += this->size().width() + 20;
  GetCurrentLayout().setWidth(sz);
  this->setGeometry(QRect(20, 10, 650, 750));
  this->append(">>");
  this->moveCursor(QTextCursor::End);
  this->installEventFilter(this);
  connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(CursorPositionChangeSlot()));
  write_enable_ = true;
  cursor_at_cmd_head_ = true;

  text_ = text;
  onenter_ = onenter;

  isolate_ = isolate;
  context_ = isolate->GetCurrentContext();

  js_self_ = args.GetReturnValue().Get()->ToObject(context_).ToLocalChecked();
  js_self_->Set(MakeStr(isolate, "onenter_func"), (onenter));
}


void TextEdit::EnterCallback() {
  QString allCmd(this->toPlainText());
  int last = allCmd.lastIndexOf(">>");
  QString lastCmd = allCmd.right(allCmd.length() - last - 2);
  //lastCmd = lastCmd.replace('\n', '\\');
  Local<Value> argv[] = { MakeStr(isolate_, lastCmd.toStdString().c_str()) };
  js_self_ = v8pp::class_<TextEdit>::find_object(isolate_, this);
  onenter_ = Local<Function>::Cast(js_self_->Get(MakeStr(isolate_, "onenter_func")));
  onenter_->CallAsFunction(isolate_->GetCurrentContext(), js_self_, 1, argv);
  this->append(">>");
  this->moveCursor(QTextCursor::End);
}

Local<FunctionTemplate> TextEdit::me_class_;
v8pp::class_<TextEdit>* TextEdit::self_class_ = nullptr;

void TextEdit::Init(Local<Object> mod, V8Shell* shell)
{
  self_class_ = new
    v8pp::class_<TextEdit>(shell->GetIsolate());
  self_class_
    ->ctor<const FunctionCallbackInfo<Value>&>()
    .set("text", &TextEdit::text_)
    .set("onenter", &TextEdit::onenter_);

  mod->Set(MakeStr(shell->GetIsolate(), "TextEdit"), self_class_->js_function_template()->GetFunction(shell->GetIsolate()->GetCurrentContext()).ToLocalChecked());
}

void TextEdit::CursorPositionChangeSlot()
{
  QTextCursor cursor = this->textCursor();
  int lastCmdPos = this->toPlainText().lastIndexOf(">>") + 2;
  if (cursor.position() < lastCmdPos) {
    write_enable_ = false;
    cursor_at_cmd_head_ = false;
  }
  else if (cursor.position() == lastCmdPos) {
    write_enable_ = true;
    cursor_at_cmd_head_ = true;
  }
  else if (cursor.position() > lastCmdPos) {
    write_enable_ = true;
    cursor_at_cmd_head_ = false;
  }
}

bool TextEdit::eventFilter(QObject* obj, QEvent* event)
{
  static bool shiftPressed = false;
  if (obj == this) {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    if (event->type() == QEvent::KeyPress) {
      if (!write_enable_ &&
        ((keyEvent->key() >= Qt::Key_Space &&
          keyEvent->key() <= Qt::Key_AsciiTilde) ||
          keyEvent->key() == Qt::Key_Enter ||
          keyEvent->key() == Qt::Key_Return ||
          keyEvent->key() == Qt::Key_Backspace ||
          keyEvent->key() == Qt::Key_Delete ||
          keyEvent->key() == Qt::Key_Tab))
        return true;
      if (cursor_at_cmd_head_ &&
        keyEvent->key() == Qt::Key_Backspace)
        return true;
      if (keyEvent->key() == Qt::Key_Shift)
        shiftPressed = true;
      else if (!shiftPressed &&
        (keyEvent->key() == Qt::Key_Enter ||
          keyEvent->key() == Qt::Key_Return)) {
        EnterCallback();
        return true;
      }
    }
    else if (event->type() == QEvent::KeyRelease) {
      if (keyEvent->key() == Qt::Key_Shift) {
        shiftPressed = false;
      }
    }
  }
  return QObject::eventFilter(obj, event);
}