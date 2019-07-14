#pragma once

#include<QWidget>
#include<QPainter>
#include<QMouseEvent>
#include<QApplication>

struct PenSetting
{
	int size;
	int R;
	int G;
	int B;
};

class Mywidget : public QWidget
{
	Q_OBJECT
public:
	explicit Mywidget(QWidget* parent = 0);

	void paintEvent(QPaintEvent* ev);
	void mouseMoveEvent(QMouseEvent* ev);
	void mousePressEvent(QMouseEvent* ev);
	void mouseReleaseEvent(QMouseEvent* ev);
};