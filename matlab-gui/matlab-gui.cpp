#include "matlab-gui.h"
#include <QButtonGroup>

MatlabGui::MatlabGui(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::MatlabGuiClass)
{
	ui->setupUi(this);
	
	connect(ui->contrast, SIGNAL(valueChanged(int)), this, SLOT(onPicChanges()));
	connect(ui->brightness, SIGNAL(valueChanged(int)), this, SLOT(onPicChanges()));
	connect(ui->rotate, SIGNAL(valueChanged(int)), this, SLOT(onPicChanges()));
	connect(ui->width, SIGNAL(valueChanged(int)), this, SLOT(onPicChanges()));
	connect(ui->height, SIGNAL(valueChanged(int)), this, SLOT(onPicChanges()));
	connect(ui->face, SIGNAL(stateChanged(int)), this, SLOT(onPicChanges()));
	connect(ui->equalize, SIGNAL(stateChanged(int)), this, SLOT(onPicChanges()));
	connect(ui->bin, SIGNAL(stateChanged(int)), this, SLOT(onPicNew()));
	connect(ui->gray, SIGNAL(stateChanged(int)), this, SLOT(onPicNew()));


	auto scene1 = new QGraphicsScene(this);
	ui->view1->setScene(scene1);
	auto pix1 = new QPixmap("test-js/Lena.bmp");
	scene1->addPixmap(*pix1);
	ui->view1->show();

	auto scene2 = new QGraphicsScene(this);
	ui->view2->setScene(scene2);
	auto pix2 = new QPixmap("test-out/contrast.png");
	scene2->addPixmap(*pix2);
	ui->view2->show();
}


void MatlabGui::onPicChanges()
{
	double contrast = 0.02 * ui->contrast->value(); // 0.0x ~ 2.0x
	int brightness = ui->brightness->value();
	int degree = ui->rotate->value();
	int widthFactor = ui->width->value();
	int heightFactor = ui->height->value();
	bool face = ui->face->isChecked();
	bool equalize = ui->equalize->isChecked();

	auto temp = new Matrix();
	temp->matrix = cv::imread("test-js/Lena.bmp");
	temp->linear(contrast, brightness);
	temp->rotate(degree);

	std::vector<int> v;
	v.push_back(widthFactor);
	v.push_back(heightFactor);
	temp->resize(v);
	if(equalize)
		temp->equalizeHist();
	if(face)
		temp->face();
	
	cv::imwrite("test-out/out.png", temp->matrix);

	auto tempScene = new QGraphicsScene(this);
	ui->view2->setScene(tempScene);
	auto tempPix = new QPixmap("test-out/out.png");
	tempScene->addPixmap(*tempPix);
	ui->view2->show();
}

void MatlabGui::onPicNew()
{
	bool gray = ui->gray->isChecked();
	bool bin = ui->bin->isChecked();

	auto temp = new Matrix();
	temp->matrix = cv::imread("test-out/out.png");

	if (gray)
		temp->togray();
	if (bin)
		temp->tobin();

	cv::imwrite("test-out/new.png", temp->matrix);

	auto tempScene = new QGraphicsScene(this);
	ui->view2->setScene(tempScene);
	auto tempPix = new QPixmap("test-out/new.png");
	tempScene->addPixmap(*tempPix);
	ui->view2->show();
}