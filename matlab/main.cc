#include <functional>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <assert.h>

#include "js/shell.h"



int main(int argc, char* argv[])
{

	V8Shell shell(argc, argv, std::cerr);

	while (!shell.Closed())
	{
		std::cout << "> ";
		std::string str;
		std::getline(std::cin, str);
		shell.Execute(str);
	}

	return 0;
}