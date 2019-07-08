#include <functional>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <assert.h>

#include "js/shell.h"
#include "js/matrix.h"

#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

bool CheckEscapeLine(std::string& str);


int main(int argc, char* argv[])
{

	V8Shell shell(argc, argv, std::cerr);
	DefineJSMatrix(&shell);


	//if (fs::exists("__init__.js")) {
	//	std::ifstream ifs("__init__.js");
	//
	//	std::string str = std::string(
	//		std::istreambuf_iterator<char>(ifs),
	//		std::istreambuf_iterator<char>()
	//	);
	//
	//	shell.Execute(str);
	//}

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

bool CheckEscapeLine(std::string& str) {
	int sz = str.size();
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