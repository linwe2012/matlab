#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_matlab-gui.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include "../matlab-core/matlab.h"

class MatlabGui : public QMainWindow
{
	Q_OBJECT

public:
	MatlabGui(QWidget *parent = Q_NULLPTR);

private slots:
	void onPicChanges();
	void onPicNew();

private:
	Ui::MatlabGuiClass *ui;
};
