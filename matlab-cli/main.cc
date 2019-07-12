#include <functional>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <assert.h>

//#include "js/shell.h"
//#include "js/matrix.h"
#include "../matlab-core/matlab.h"
#include "../matlab-core/js/convert-utils.h"

#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

bool CheckEscapeLine(std::string& str);

//#define ENABLE_GUI

#ifndef ENABLE_GUI

struct TestClass {
	TestClass(int x, double d, int v)
		:x(x), d(d), v(v)
	{}
	int x;
	double d;
	int v;
};
int main(int argc, char* argv[])
{

	V8Shell shell(argc, argv, std::cerr);
	DefineJSMatrix(&shell);
	
	v8pp::class_<TestClass> tc(shell.GetIsolate());
	tc
		.ctor<int, double, int>()
		.set("x", &TestClass::x)
		.set("d", &TestClass::d)
		.set("v", &TestClass::v);

	auto obj = tc.create_object(shell.GetIsolate(), 1, 2, 3);
	const char* names[] = { "x", "d", "v" };

	auto [x, d, v] =
		GetProperties<int, double, int>(shell.GetIsolate(), obj, names);

	std::cout << x << " " << d << " " << v;
	while (!shell.Closed())
	{
		std::cout << ">> ";

		std::string str;
		std::getline(std::cin, str);

		while (CheckEscapeLine(str))
		{
			std::cout << " > ";
			std::string tmp;
			std::getline(std::cin, tmp);

			str += tmp;
		}
		shell.Execute(str);
	}

	return 0;
}

#endif // !



bool CheckEscapeLine(std::string& str) {
	int sz = static_cast<int>(str.size());
	if (sz == 0) {
		return false;
	}
	--sz;
	while (sz >= 0 && str[sz] == '\n' || str[sz] == '\r')
	{
		--sz;
	}

	if (str[sz] == '\\') {
		str.erase(sz);
		return true;
	}
	return false;
}