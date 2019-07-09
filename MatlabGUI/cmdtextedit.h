#ifndef CMDTEXTEDIT_H
#define CMDTEXTEDIT_H

#include <QWidget>
#include <QDebug>
#include <QTextEdit>
#include <QBoxLayout>
#include <QEvent>
#include <QKeyEvent>

class CmdTextEdit : public QWidget
{
  Q_OBJECT
public:
  explicit CmdTextEdit(QWidget *parent = nullptr);
  ~CmdTextEdit();
  QTextEdit *GetTextEdit()
  { return text_edit_; }

private:
  QTextEdit *text_edit_;
  int cmd_start_pos_;
signals:
  void CmdEnter();
private slots:
  void CursorPositionChangeSlot();
  bool eventFilter(QObject *obj, QEvent *event);
};

#endif // CMDTEXTEDIT_H
