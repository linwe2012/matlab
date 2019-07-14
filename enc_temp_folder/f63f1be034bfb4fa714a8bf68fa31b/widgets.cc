#include "../matlab-core/matlab.h"
#include "../matlab-core/matlab-experimental.h"
#include "matlab-gui.h"
#include "qpushbutton.h"
#include "button.h"
#include "widgets.h"
#include "widget-helper.h"
#include <QFileDialog>
#include "fileselector.h"
#include "layout.h"
#include "slider.h"
#include "colorpicker.h"
using namespace v8;



void ReigsterGui(V8Shell* shell);


QWidget* target_widgets;


// NOTE: This does not cover all cases - it should be easy to add new ones as required.
inline QImage  cvMatToQImage(const cv::Mat& inMat)
{
	switch (inMat.type())
	{
		// 8-bit, 4 channel
	case CV_8UC4:
	{
		QImage image(inMat.data,
			inMat.cols, inMat.rows,
			static_cast<int>(inMat.step),
			QImage::Format_ARGB32);

		return image;
	}

	// 8-bit, 3 channel
	case CV_8UC3:
	{
		QImage image(inMat.data,
			inMat.cols, inMat.rows,
			static_cast<int>(inMat.step),
			QImage::Format_RGB888);

		return image.rgbSwapped();
	}

	// 8-bit, 1 channel
	case CV_8UC1:
	{
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
		QImage image(inMat.data,
			inMat.cols, inMat.rows,
			static_cast<int>(inMat.step),
			QImage::Format_Grayscale8);
#else
		static QVector<QRgb>  sColorTable;

		// only create our color table the first time
		if (sColorTable.isEmpty())
		{
			sColorTable.resize(256);

			for (int i = 0; i < 256; ++i)
			{
				sColorTable[i] = qRgb(i, i, i);
			}
		}

		QImage image(inMat.data,
			inMat.cols, inMat.rows,
			static_cast<int>(inMat.step),
			QImage::Format_Indexed8);

		image.setColorTable(sColorTable);
#endif

		return image;
	}

	default:
		qWarning("ASM::cvMatToQImage() - cv::Mat image type not handled in switch: %s", inMat.type());
		break;
	}

	return QImage();
}

struct GuiModule {
	void Display(const FunctionCallbackInfo<Value>& args) {
		Matrix *mat = 
		v8pp::class_<Matrix>::unwrap_object(args.GetIsolate(), args[0]);

		auto& img = mat->matrix;
		image_scene->addPixmap(QPixmap::fromImage(cvMatToQImage(img)));
		image_view->setScene(image_scene);
		image_view->setParent(target_widgets);
		image_view->setGeometry(QRect(20, 120, 800, 800));
		image_view->show();
	}

	void FileDiaglog(const FunctionCallbackInfo<Value>& args) {
		INIT_OBJECT(args);
		QFileDialog* fileDialog = new QFileDialog(target_widgets);
		GET(std::string, baseDir);
		GET(Local<Function>, callback);
		GET(std::string, nameFilter);
		GET(std::string, title);

		fileDialog->setWindowTitle(title.c_str());
		fileDialog->setDirectory(baseDir.c_str());
		fileDialog->setNameFilter(nameFilter.c_str());
		fileDialog->setViewMode(QFileDialog::Detail);
		QStringList fileNames;
		if (fileDialog->exec())
		{
			fileNames = fileDialog->selectedFiles();
		}
		Local<Array> arr = Array::New(isolate, fileNames.size());
		int i = 0;
		for (auto f : fileNames) {
			arr->Set(i, MakeStr(isolate, f.toUtf8().constData()));
		}
		Local<Value> argv[] = { arr };
		int argc = 1;
		callback->CallAsFunction(context, js_self_, 1, argv);
		delete fileDialog;
	}
    void SaveAsDialog(const FunctionCallbackInfo<Value>& args) {
      INIT_OBJECT(args);
      QFileDialog* fileDialog = new QFileDialog(target_widgets);
      GET(std::string, baseDir);
      GET(Local<Function>, callback);
      GET(std::string, nameFilter);
      GET(std::string, title);
      GET_IF(std::string, name, "");
      fileDialog->setWindowTitle(title.c_str());
      fileDialog->setDirectory(baseDir.c_str());
      fileDialog->setNameFilter(nameFilter.c_str());
      fileDialog->setViewMode(QFileDialog::Detail);
      fileDialog->setAcceptMode(QFileDialog::AcceptSave);
      fileDialog->setOptions(QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
      fileDialog->selectFile(name.c_str());
      QStringList fileNames;
      if (fileDialog->exec())
      {
        fileNames = fileDialog->selectedFiles();
      }
      Local<Array> arr = Array::New(isolate, fileNames.size());
      int i = 0;
      for (auto f : fileNames) {
        arr->Set(i, MakeStr(isolate, f.toUtf8().constData()));
      }
      Local<Value> argv[] = { arr };
      int argc = 1;
      callback->CallAsFunction(context, js_self_, 1, argv);
      delete fileDialog;
    }

	QGraphicsView* image_view;
	QGraphicsScene* image_scene;
	Local<Object> js_self_;
};

  

void ReigsterGui(V8Shell* shell, QMainWindow* main)
{
	v8pp::class_<GuiModule> gui(shell->GetIsolate());
	gui
		.set("display", &GuiModule::Display)
		.set("fileDialog", &GuiModule::FileDiaglog)
		;
	Local<Object> mod = gui.create_object(shell->GetIsolate());
	gui.unwrap_object(shell->GetIsolate(), mod)->js_self_ = mod;
	Layout::Init(mod, shell);

	ColorPicker::Init(mod, shell);
	Slider::Init(mod, shell);
	Button::Init(mod, shell);
	JSObjVeiwer::Init(mod, shell);
	GuiModule& pmod = *gui.unwrap_object(shell->GetIsolate(), mod);
	
	
	shell->RegisterGlobals(
		{
			{"gui", mod}
		}
	);

	pmod.image_view = new QGraphicsView(target_widgets);
	pmod.image_view->setObjectName(QString::fromUtf8("image_view"));
	pmod.image_view->setGeometry(QRect(20, 50, 421, 321));

	pmod.image_scene = new QGraphicsScene(main);

	pmod.image_view->setScene(pmod.image_scene);
	// pmod.splitter_lr = new QSplitter(Qt::Horizontal, GetTargetWidget());

	target_widgets = new QWidget(main);
	target_widgets->setObjectName(QString::fromUtf8("toolbar"));
	main->setCentralWidget(target_widgets);
	// QMetaObject::connectSlotsByName(main);
}

QWidget* GetTargetWidget()
{
	return target_widgets;
}

QSize current_layout(0, 0);
QSize& GetCurrentLayout()
{
	return current_layout;
}


