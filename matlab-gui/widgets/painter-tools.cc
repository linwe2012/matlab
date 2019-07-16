#include "painter-tools.h"
#include "registry.h"
#include "widget-helper.h"
v8pp::class_<PainterTool>* PainterTool::self_class_ = nullptr;

PainterTool::PainterTool(const FunctionCallbackInfo<Value>& args)
{
	setParent(GetImageView());
	shapeComboBox = new QComboBox;
	shapeLabel = new QLabel(tr("&Shape:"));
	shapeLabel->setBuddy(shapeComboBox);

	penWidthSpinBox = new QSpinBox;
	penWidthSpinBox->setRange(0, 20);
	penWidthSpinBox->setSpecialValueText(tr("0 (cosmetic pen)"));

	penWidthLabel = new QLabel(tr("Pen &Width:"));
	penWidthLabel->setBuddy(penWidthSpinBox);

	penStyleComboBox = new QComboBox;
	penStyleComboBox->addItem(tr("Solid"), static_cast<int>(Qt::SolidLine));
	penStyleComboBox->addItem(tr("Dash"), static_cast<int>(Qt::DashLine));
	penStyleComboBox->addItem(tr("Dot"), static_cast<int>(Qt::DotLine));
	penStyleComboBox->addItem(tr("Dash Dot"), static_cast<int>(Qt::DashDotLine));
	penStyleComboBox->addItem(tr("Dash Dot Dot"), static_cast<int>(Qt::DashDotDotLine));
	penStyleComboBox->addItem(tr("None"), static_cast<int>(Qt::NoPen));

	penStyleLabel = new QLabel(tr("&Pen Style:"));
	penStyleLabel->setBuddy(penStyleComboBox);

	penCapComboBox = new QComboBox;
	penCapComboBox->addItem(tr("Flat"), Qt::FlatCap);
	penCapComboBox->addItem(tr("Square"), Qt::SquareCap);
	penCapComboBox->addItem(tr("Round"), Qt::RoundCap);

	penCapLabel = new QLabel(tr("Pen &Cap:"));
	penCapLabel->setBuddy(penCapComboBox);

	penJoinComboBox = new QComboBox;
	penJoinComboBox->addItem(tr("Miter"), Qt::MiterJoin);
	penJoinComboBox->addItem(tr("Bevel"), Qt::BevelJoin);
	penJoinComboBox->addItem(tr("Round"), Qt::RoundJoin);

	penJoinLabel = new QLabel(tr("Pen &Join:"));
	penJoinLabel->setBuddy(penJoinComboBox);

	brushStyleComboBox = new QComboBox;
	brushStyleComboBox->addItem(tr("Linear Gradient"),
		static_cast<int>(Qt::LinearGradientPattern));
	brushStyleComboBox->addItem(tr("Radial Gradient"),
		static_cast<int>(Qt::RadialGradientPattern));
	brushStyleComboBox->addItem(tr("Conical Gradient"),
		static_cast<int>(Qt::ConicalGradientPattern));
	brushStyleComboBox->addItem(tr("Texture"), static_cast<int>(Qt::TexturePattern));
	brushStyleComboBox->addItem(tr("Solid"), static_cast<int>(Qt::SolidPattern));
	brushStyleComboBox->addItem(tr("Horizontal"), static_cast<int>(Qt::HorPattern));
	brushStyleComboBox->addItem(tr("Vertical"), static_cast<int>(Qt::VerPattern));
	brushStyleComboBox->addItem(tr("Cross"), static_cast<int>(Qt::CrossPattern));
	brushStyleComboBox->addItem(tr("Backward Diagonal"), static_cast<int>(Qt::BDiagPattern));
	brushStyleComboBox->addItem(tr("Forward Diagonal"), static_cast<int>(Qt::FDiagPattern));
	brushStyleComboBox->addItem(tr("Diagonal Cross"), static_cast<int>(Qt::DiagCrossPattern));
	brushStyleComboBox->addItem(tr("Dense 1"), static_cast<int>(Qt::Dense1Pattern));
	brushStyleComboBox->addItem(tr("Dense 2"), static_cast<int>(Qt::Dense2Pattern));
	brushStyleComboBox->addItem(tr("Dense 3"), static_cast<int>(Qt::Dense3Pattern));
	brushStyleComboBox->addItem(tr("Dense 4"), static_cast<int>(Qt::Dense4Pattern));
	brushStyleComboBox->addItem(tr("Dense 5"), static_cast<int>(Qt::Dense5Pattern));
	brushStyleComboBox->addItem(tr("Dense 6"), static_cast<int>(Qt::Dense6Pattern));
	brushStyleComboBox->addItem(tr("Dense 7"), static_cast<int>(Qt::Dense7Pattern));
	brushStyleComboBox->addItem(tr("None"), static_cast<int>(Qt::NoBrush));

	color_dialog = new QColorDialog;

	brushStyleLabel = new QLabel(tr("&Brush:"));
	brushStyleLabel->setBuddy(brushStyleComboBox);

	otherOptionsLabel = new QLabel(tr("Options:"));
	antialiasingCheckBox = new QCheckBox(tr("&Antialiasing"));

	transformationsCheckBox = new QCheckBox(tr("&Transformations"));

	//connect(shapeComboBox, QOverload<int>::of(&QComboBox::activated),
		//this, &shapeChanged);
	connect(penWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
		this, &PainterTool::penChanged);
	
	connect(penStyleComboBox, QOverload<int>::of(&QComboBox::activated),
		this, &PainterTool::penChanged);
	connect(penCapComboBox, QOverload<int>::of(&QComboBox::activated),
		this, &PainterTool::penChanged);
	connect(penJoinComboBox, QOverload<int>::of(&QComboBox::activated),
		this, &PainterTool::penChanged);
	connect(brushStyleComboBox, QOverload<int>::of(&QComboBox::activated),
		this, &PainterTool::brushChanged);

	connect(color_dialog, QOverload<const QColor&>::of(&QColorDialog::currentColorChanged),
		this, &PainterTool::colorChanged);

	QGridLayout* mainLayout = new QGridLayout;
	mainLayout->setColumnStretch(0, 1);
	mainLayout->setColumnStretch(1, 1);
	
	int i = 0;
	mainLayout->addWidget(color_dialog, 0, 0, 10, 2);
	int col = 10;
	mainLayout->addWidget(shapeLabel, col +1, 0, Qt::AlignRight);
	mainLayout->addWidget(shapeComboBox, col + 1, 1);
	mainLayout->addWidget(penWidthLabel, col + 2, 0, Qt::AlignRight);
	mainLayout->addWidget(penWidthSpinBox, col + 2, 1);
	mainLayout->addWidget(penStyleLabel, col + 3, 0, Qt::AlignRight);
	mainLayout->addWidget(penStyleComboBox, col + 3, 1);
	mainLayout->addWidget(penCapLabel, col + 4, 0, Qt::AlignRight);
	mainLayout->addWidget(penCapComboBox, col + 4, 1);
	mainLayout->addWidget(penJoinLabel, col + 5, 0, Qt::AlignRight);
	mainLayout->addWidget(penJoinComboBox, col + 5, 1);
	mainLayout->addWidget(brushStyleLabel, col + 6, 0, Qt::AlignRight);
	mainLayout->addWidget(brushStyleComboBox, col + 6, 1);
	mainLayout->addWidget(otherOptionsLabel, col + 7, 0, Qt::AlignRight);
	mainLayout->addWidget(antialiasingCheckBox, col + 7, 1, 1, 1);
	mainLayout->addWidget(transformationsCheckBox, col + 8, 1, 1, 1);
	// setLayout(mainLayout);
	
	shapeChanged();
	penChanged();
	brushChanged();
	antialiasingCheckBox->setChecked(true);

	setWindowTitle(tr("Basic Drawing"));
	QWidget* toolbox = new QWidget (GetTargetWidget());

	toolbox->setGeometry(QRect(830, 80, 250, 650));
	toolbox->setLayout(mainLayout);
	//this->show();
	//this->setBackgroundRole(QPalette::Dark);
	//connect(antialiasingCheckBox, &QAbstractButton::toggled,
		//renderArea, &RenderArea::setAntialiased);
	//connect(transformationsCheckBox, &QAbstractButton::toggled,
		//renderArea, &RenderArea::setTransformed);
	INIT_OBJECT(args);
	GET(Local<Function>, onrelease);
	isolate_ = isolate;
	js_self_ = args.GetReturnValue().Get()->ToObject(context_).ToLocalChecked();
	js_self_->Set(MakeStr(isolate, "onrelease_func"), (onrelease));
}

void PainterTool::shapeChanged()
{

}
const int IdRole = Qt::UserRole;
void PainterTool::penChanged()
{
	int width = penWidthSpinBox->value();
	Qt::PenStyle style = Qt::PenStyle(penStyleComboBox->itemData(
		penStyleComboBox->currentIndex(), IdRole).toInt());
	Qt::PenCapStyle cap = Qt::PenCapStyle(penCapComboBox->itemData(
		penCapComboBox->currentIndex(), IdRole).toInt());
	Qt::PenJoinStyle join = Qt::PenJoinStyle(penJoinComboBox->itemData(
		penJoinComboBox->currentIndex(), IdRole).toInt());
	
	pen = QPen(Qt::blue, width, style, cap, join);
	// renderArea->setPen(QPen(Qt::blue, width, style, cap, join));
}
void PainterTool::brushChanged()
{

	Qt::BrushStyle style = Qt::BrushStyle(
			brushStyleComboBox->itemData(brushStyleComboBox->currentIndex(), IdRole).toInt()
		);

	if (style == Qt::LinearGradientPattern) {
		QLinearGradient linearGradient(0, 0, 100, 100);
		linearGradient.setColorAt(0.0, Qt::white);
		linearGradient.setColorAt(0.2, Qt::green);
		linearGradient.setColorAt(1.0, Qt::black);
		brush = QBrush(linearGradient);
		// renderArea->setBrush(linearGradient);
	}
	else if (style == Qt::RadialGradientPattern) {
		QRadialGradient radialGradient(50, 50, 50, 70, 70);
		radialGradient.setColorAt(0.0, Qt::white);
		radialGradient.setColorAt(0.2, Qt::green);
		radialGradient.setColorAt(1.0, Qt::black);
		brush = QBrush(radialGradient);
		//renderArea->setBrush(radialGradient);
	}
	else if (style == Qt::ConicalGradientPattern) {
		QConicalGradient conicalGradient(50, 50, 150);
		conicalGradient.setColorAt(0.0, Qt::white);
		conicalGradient.setColorAt(0.2, Qt::green);
		conicalGradient.setColorAt(1.0, Qt::black);
		brush = QBrush(conicalGradient);
		//renderArea->setBrush(conicalGradient);
	}
	else {
		brush = QBrush(Qt::green, style);
		//renderArea->setBrush(QBrush(Qt::green, style));
	}
}

void PainterTool::colorChanged(const QColor& c)
{
	pen_color = c;
}

inline cv::Mat QImageToCvMat(const QImage& inImage, bool inCloneImageData = true)
{
	switch (inImage.format())
	{
		// 8-bit, 4 channel
	case QImage::Format_ARGB32:
	case QImage::Format_ARGB32_Premultiplied:
	{
		cv::Mat  mat(inImage.height(), inImage.width(),
			CV_8UC4,
			const_cast<uchar*>(inImage.bits()),
			static_cast<size_t>(inImage.bytesPerLine())
		);

		return (inCloneImageData ? mat.clone() : mat);
	}

	// 8-bit, 3 channel
	case QImage::Format_RGB32:
	{
		if (!inCloneImageData)
		{
			//qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
		}

		cv::Mat  mat(inImage.height(), inImage.width(),
			CV_8UC4,
			const_cast<uchar*>(inImage.bits()),
			static_cast<size_t>(inImage.bytesPerLine())
		);

		cv::Mat  matNoAlpha;

		cv::cvtColor(mat, matNoAlpha, cv::COLOR_BGRA2BGR);   // drop the all-white alpha channel

		return matNoAlpha;
	}

	// 8-bit, 3 channel
	case QImage::Format_RGB888:
	{
		if (!inCloneImageData)
		{
			//qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
		}

		QImage   swapped = inImage.rgbSwapped();

		return cv::Mat(swapped.height(), swapped.width(),
			CV_8UC3,
			const_cast<uchar*>(swapped.bits()),
			static_cast<size_t>(swapped.bytesPerLine())
		).clone();
	}

	// 8-bit, 1 channel
	case QImage::Format_Indexed8:
	{
		cv::Mat  mat(inImage.height(), inImage.width(),
			CV_8UC1,
			const_cast<uchar*>(inImage.bits()),
			static_cast<size_t>(inImage.bytesPerLine())
		);

		return (inCloneImageData ? mat.clone() : mat);
	}

	default:
		//qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
		break;
	}

	return cv::Mat();
}

// If inPixmap exists for the lifetime of the resulting cv::Mat, pass false to inCloneImageData to share inPixmap's data
// with the cv::Mat directly
//    NOTE: Format_RGB888 is an exception since we need to use a local QImage and thus must clone the data regardless
inline cv::Mat QPixmapToCvMat(const QPixmap& inPixmap, bool inCloneImageData = true)
{
	return QImageToCvMat(inPixmap.toImage(), inCloneImageData);
}


void PainterTool::Init(Local<Object> mod, V8Shell* shell)
{
	self_class_ = new
		v8pp::class_<PainterTool>(shell->GetIsolate());
	self_class_
		->ctor<const FunctionCallbackInfo<Value>&>();

	mod->Set(MakeStr(shell->GetIsolate(), "PainterTool"), self_class_->js_function_template()->GetFunction(shell->GetIsolate()->GetCurrentContext()).ToLocalChecked());
}

void PainterTool::paintEvent(QPaintEvent*)
{
	if (!is_painting) {
		return;
	}

	//QImage tmp(GetPixelMap()->toImage());
	//QPainter p(&tmp);
	QPainter p(GetPixelMap());
	p.setRenderHint(QPainter::Antialiasing, true);
	brush.setColor(pen_color);
	pen.setColor(pen_color);

	QPoint last;
	bool has_last = false;
	for (int i = 0; i < lines_.size(); i++)
	{
		p.setPen(pen);
		p.setBrush(brush);
		// p.setPen(QPen(QColor(pens_.at(i).R, pens_.at(i).G, pens_.at(i).B), pens_.at(i).size));
		const QVector<QPoint>& line = lines_.at(i);
		for (int j = 0; j < line.size() - 1; j++) {
			p.drawLine(line.at(j), line.at(j + 1));
			last = line.at(j + 1);
			has_last = true;
		}
	}


	//*GetPixelMap() = QPixmap::fromImage(tmp);
	lines_.clear();

	QVector<QPoint> points;
	points.append(last);
	points.append(last_p);

	lines_.append(points);
	//lines_.append(QVector<QPoint>{p});

	ReRenderPixelMap();
	//lines_.clear();
}

void PainterTool::mouseMoveEvent(QMouseEvent* ev)
{
	if (lines_.size() == 0)
	{
		QVector<QPoint> line;
		lines_.append(line);
	}
	
	QVector<QPoint>& lastLine = lines_.last();
	lastLine.append(ev->pos());
	last_p = ev->pos();
	is_painting = true;
	update();
}

void PainterTool::mousePressEvent(QMouseEvent* ev)
{
	QVector<QPoint> line;
	lines_.append(line);

	PenSetting settings;
	
	settings.size = 20;
	settings.R = 255;
	settings.G = 0;
	settings.B = 0;
	pens_.append(settings);

	QVector<QPoint>& lastLine = lines_.last();
	lastLine.append(ev->pos());
	//this_p = ev->pos();
}

void PainterTool::mouseReleaseEvent(QMouseEvent* ev)
{
	//QVector<QPoint>& lastLine = lines_.last();
	//lastLine.append(ev->pos());
	Local<Value> argv[] = {
		v8pp::class_<Matrix>::import_external(isolate_, new Matrix(QPixmapToCvMat(*GetPixelMap()), nullptr))
	};
	is_painting = false;
	js_self_ = v8pp::class_ < PainterTool>::find_object(isolate_, this);
	onrelease_ = Local<Function>::Cast(js_self_->Get(MakeStr(isolate_, "onrelease_func")));
	onrelease_->CallAsFunction(isolate_->GetCurrentContext(), js_self_, 1, argv);

}
