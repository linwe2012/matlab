#include "matlab-gui.h"
#include <QtWidgets/QApplication>
#include "../matlab-core/matlab.h"
#include <iostream>


int main(int argc, char *argv[])
{
	V8Shell shell(argc, argv, std::cout);

	QApplication a(argc, argv);
	MatlabGui w;
	w.show();
	return a.exec();
}
