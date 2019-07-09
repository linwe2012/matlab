#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_matlab-gui.h"

class MatlabGui : public QMainWindow
{
	Q_OBJECT

public:
	MatlabGui(QWidget *parent = Q_NULLPTR);

private:
	Ui::MatlabGuiClass ui;
};
