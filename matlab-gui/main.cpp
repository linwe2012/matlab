#include "matlab-gui.h"
#include <QtWidgets/QApplication>
#include "../matlab-core/matlab.h"
#include <iostream>
#include "widgets/widgets.h"
#include <fstream>
class OutStream : public std::ostream {
	template<typename T>
	OutStream& operator<<(T data) {

	}
};



int main(int argc, char *argv[])
{
	std::ofstream ofs("log.txt");
	V8Shell shell(argc, argv, ofs);
	DefineJSMatrix(&shell);
	
	QApplication a(argc, argv);
	MatlabGui w;
	
	ReigsterGui(&shell, &w);
	shell.Execute("eval(shell.readtxt('test-js/button.js'))");
	w.show();
	return a.exec();
}
