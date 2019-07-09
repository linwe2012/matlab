#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  Initialize();
  this->setMinimumSize(600,400);
}

MainWindow::~MainWindow()
{

}

void MainWindow::Initialize()
{
  //initialize menu
  QMenuBar *mb = menuBar();
  QMenu *menu = new QMenu("File");
  QAction *action;
  action = new QAction("Load...");
  action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
  connect(action,SIGNAL(triggered()),this,SLOT(ActionLoadSlot()));
  menu->addAction(action);
  action = new QAction("Save");
  action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
  connect(action,SIGNAL(triggered()),this,SLOT(ActionSaveSlot()));
  menu->addAction(action);
  action = new QAction("Script Edit");
  action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
  connect(action, SIGNAL(triggered()), this, SLOT(ActionScriptEditSlot()));
  menu->addAction(action);
  mb->addMenu(menu);

  //initialize center widget
  QWidget *center = new QWidget;
  setCentralWidget(center);
  QVBoxLayout *vLay = new QVBoxLayout;
  editor_ = new QTextEdit();
  editor_->hide();
  editor_is_active_ = false;
  vLay->addWidget(editor_);
  command_ = new CmdTextEdit();
  vLay->addWidget(command_);
  connect(command_,SIGNAL(CmdEnter()),this,SLOT(CommandSlot()));
  QHBoxLayout *hLay = new QHBoxLayout;
  hLay->addLayout(vLay);
  monitor_ = new QGroupBox();
  monitor_->hide();
  hLay->addWidget(monitor_);
  center->setLayout(hLay);
}

void MainWindow::ActionLoadSlot()
{
  file_name_ = QFileDialog::getOpenFileName(this, "open file", " ", "Allfile(*.*);");
  QFile file(file_name_);
  qDebug()<<"Open "<<file_name_;
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
    qDebug()<<"File open fail";
    return;
  }
  QString display;
  while(!file.atEnd()){
    display.append(QString(file.readLine()));
    qDebug()<<display;
  }
  editor_->setPlainText(display);
  editor_->show();
  editor_is_active_ = true;
  file.close();
}

void MainWindow::ActionSaveSlot()
{
  if(!editor_is_active_)
    return;
  QFile file(file_name_);
  qDebug()<<"Open"<<file_name_;
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    qDebug()<<"File open fail";
  QString output(editor_->toPlainText());
  file.write(output.toUtf8());
}

void MainWindow::ActionScriptEditSlot()
{
  if(editor_is_active_){
    editor_->hide();
    editor_is_active_ = false;
  }
  else {
    editor_->show();
    editor_is_active_ = true;
  }
}

void MainWindow::CommandSlot()
{
  QString allCmd(command_->GetTextEdit()->toPlainText());
  int last = allCmd.lastIndexOf(">>");
  QString lastCmd = allCmd.right(allCmd.length()-last-2);
  lastCmd = lastCmd.replace('\n','\\');
  emit SendCommand(lastCmd);
  qDebug()<<lastCmd;
  /*Need a function that judge what to paint*/
  command_->GetTextEdit()->append(">>");
  command_->GetTextEdit()->moveCursor(QTextCursor::End);
}

