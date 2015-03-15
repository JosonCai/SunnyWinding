#include <iostream>
#include "Assert.h"

// 断言，输出一些重要信息
bool AssertFunction(bool b, char*desc, int line, char* fileName, bool *alwaysIgnore)
{
	if (!(*alwaysIgnore)) {
		std::cout << "assert is false." << std::endl;
		std::cout << desc << std::endl;
		std::cout << line << std::endl;
		std::cout << fileName << std::endl;
		return true;
	}

	return false;
}