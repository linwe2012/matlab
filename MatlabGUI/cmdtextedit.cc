#include "cmdtextedit.h"

CmdTextEdit::CmdTextEdit(QWidget *parent) : QWidget(parent)
{
  text_edit_ = new QTextEdit();
  text_edit_->setText(">>");
  text_edit_->moveCursor(QTextCursor::End);
  text_edit_->installEventFilter(this);
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->addWidget(text_edit_);
  layout->setContentsMargins(0, 0, 0, 0);
}

CmdTextEdit::~CmdTextEdit()
{

}

void CmdTextEdit::CursorPositionChangeSlot()
{

}

bool CmdTextEdit::eventFilter(QObject *obj, QEvent *event)
{
  static bool shiftPressed = false;
  if(obj==text_edit_){
      QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
      if(event->type()==QEvent::KeyPress){
        if(keyEvent->key()==Qt::Key_Shift)
          shiftPressed = true;
        else if(!shiftPressed&&
                (keyEvent->key()==Qt::Key_Enter||
                 keyEvent->key()==Qt::Key_Return)){
          emit CmdEnter();
          return true;
        }
      }
      else if(event->type()==QEvent::KeyRelease){
          if(keyEvent->key()==Qt::Key_Shift){
              shiftPressed = false;
          }
      }
  }
  return QObject::eventFilter(obj ,event);
}
