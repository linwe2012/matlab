#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTextEdit>
#include <QPushButton>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QLabel>
#include <QLineEdit>
#include <QKeySequence>
#include <QBoxLayout>
#include <QToolBar>
#include <QGroupBox>
#include <QEvent>
#include <QKeyEvent>
#include "cmdtextedit.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  CmdTextEdit* command_;
  QTextEdit* editor_;
  QGroupBox* monitor_;
  QString file_name_;
  bool editor_is_active_;
  void Initialize();

signals:
  void SendCommand(QString);
private slots:
  void ActionLoadSlot();
  void ActionSaveSlot();
  void ActionScriptEditSlot();
  void CommandSlot();
};

#endif // MAINWINDOW_H
