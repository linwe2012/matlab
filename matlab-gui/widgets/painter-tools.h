#pragma once
#include "qtoolbutton.h"
#include "matlab-gui.h"
#include "qspinbox.h"
#include "qcombobox.h"
#include <QColorDialog>

class PainterTool : public QWidget
{
public:
	std::string text_;
	Local<Function> onpress_;
	Local<Function> onmove_;
	Local<Function> onrelease_;
	Local<Object> js_self_;
	QVector<QVector<QPoint>> lines_;
	QVector<PenSetting> pens_;

	PainterTool(const FunctionCallbackInfo<Value>& args);

	static void Init(Local<Object> mod, V8Shell* shell);
	// bool eventFilter(QObject* target, QEvent* event);


	Q_OBJECT

private slots:
	void penChanged();
	void shapeChanged();
	void brushChanged();
	void colorChanged(const QColor&);

private:
	void paintEvent(QPaintEvent* ev);
	void mouseMoveEvent(QMouseEvent* ev);
	void mousePressEvent(QMouseEvent* ev);
	void mouseReleaseEvent(QMouseEvent* ev);

	Isolate* isolate_;
	Local<Context> context_;
	//static Local<FunctionTemplate> me_class_;
	static v8pp::class_<PainterTool>* self_class_;

	QLabel* shapeLabel;
	QLabel* penWidthLabel;
	QLabel* penStyleLabel;
	QLabel* penCapLabel;
	QLabel* penJoinLabel;
	QLabel* brushStyleLabel;
	QLabel* otherOptionsLabel;
	QComboBox* shapeComboBox;
	QSpinBox* penWidthSpinBox;
	QComboBox* penStyleComboBox;
	QComboBox* penCapComboBox;
	QComboBox* penJoinComboBox;
	QComboBox* brushStyleComboBox;
	QCheckBox* antialiasingCheckBox;
	QCheckBox* transformationsCheckBox;
	QPen pen;
	QBrush brush;
	QPoint last_p;
	QPoint this_p;
	QColorDialog* color_dialog;
	QColor pen_color;
	bool is_painting = false;
	//static PainterTool* active_;
};