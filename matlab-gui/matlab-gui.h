#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_matlab-gui.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include "../matlab-core/matlab.h"
#include "paintingWidget.h"



class MatlabGui : public QMainWindow
{
	Q_OBJECT

public:
	MatlabGui(QWidget *parent = Q_NULLPTR);

	static int penSize;
	static int penColorR;
	static int penColorG;
	static int penColorB;
	static QVector<QVector<QPoint>> _lines;
	static QVector<PenSetting> _pen;

private slots:
	void onPicChanges();
	void onPicNew();
	void onPenChanges();
	void onCancelling();

private:
	Ui::MatlabGuiClass *ui;
};