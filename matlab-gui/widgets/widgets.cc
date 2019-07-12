#include "../matlab-core/matlab.h"
#include "../matlab-core/matlab-experimental.h"
#include "matlab-gui.h"
#include "qpushbutton.h"
#include "button.h"
#include "widgets.h"
#include "widget-helper.h"

using namespace v8;



void ReigsterGui(V8Shell* shell);


QWidget* target_widgets;

struct GuiModule {
	void Display(const FunctionCallbackInfo<Value>& args) {
		Matrix *mat = 
		v8pp::class_<Matrix>::unwrap_object(args.GetIsolate(), args[0]);

		auto& img = mat->matrix;
		image_scene->addPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));
		image_view->show();
	}

	QGraphicsView* image_view;
	QGraphicsScene* image_scene;
};

void ReigsterGui(V8Shell* shell, QMainWindow* main)
{
	v8pp::class_<GuiModule> gui(shell->GetIsolate());
	gui
		.set("display", &GuiModule::Display)
		;
	Local<Object> mod = gui.create_object(shell->GetIsolate());

	Button::Init(mod, shell);
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


