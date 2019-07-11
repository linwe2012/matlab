#include "matlab-gui.h"
#include <QtWidgets/QApplication>

#include "../matlab-core/matlab.h"
#include <iostream>
#include "widgets/widgets.h"

int main(int argc, char *argv[])
{
	V8Shell shell(argc, argv, std::cout);
	DefineJSMatrix(&shell);
	
	QApplication a(argc, argv);
	MatlabGui w;
	
	ReigsterGui(&shell, &w);
	shell.Execute("eval(shell.readtxt('test-js/button.js'))");
	w.show();
	return a.exec();
}
