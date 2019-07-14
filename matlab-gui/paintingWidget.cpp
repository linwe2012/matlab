#include "paintingWidget.h"
#include "matlab-gui.h"

int MatlabGui::penSize = 2;
int MatlabGui::penColorR = 255;
int MatlabGui::penColorG = 0;
int MatlabGui::penColorB = 0;
QVector<QVector<QPoint>> MatlabGui::_lines = {};
QVector<PenSetting> MatlabGui::_pen = {};

Mywidget::Mywidget(QWidget* parent) : QWidget(parent)
{

}

void Mywidget::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing, true);

	for (int i = 0; i < MatlabGui::_lines.size(); i++)
	{
		p.setPen(QPen(QColor(MatlabGui::_pen.at(i).R, MatlabGui::_pen.at(i).G, MatlabGui::_pen.at(i).B), MatlabGui::_pen.at(i).size));
		const QVector<QPoint>& line = MatlabGui::_lines.at(i);
		for (int j = 0; j < line.size() - 1; j++) {
			p.drawLine(line.at(j), line.at(j + 1));
		}
	}
}

void Mywidget::mouseMoveEvent(QMouseEvent* ev)
{
	if (MatlabGui::_lines.size() == 0)
	{
		QVector<QPoint> line;
		MatlabGui::_lines.append(line);
	}

	QVector<QPoint>& lastLine = MatlabGui::_lines.last();
	lastLine.append(ev->pos());
	update();
}

void Mywidget::mousePressEvent(QMouseEvent* ev)
{
	QVector<QPoint> line;
	MatlabGui::_lines.append(line);

	PenSetting settings;
	settings.size = MatlabGui::penSize;
	settings.R = MatlabGui::penColorR;
	settings.G = MatlabGui::penColorG;
	settings.B = MatlabGui::penColorB;
	MatlabGui::_pen.append(settings);

	QVector<QPoint>& lastLine = MatlabGui::_lines.last();
	lastLine.append(ev->pos());
}

void Mywidget::mouseReleaseEvent(QMouseEvent* ev)
{
	QVector<QPoint>& lastLine = MatlabGui::_lines.last();
	lastLine.append(ev->pos());
}
