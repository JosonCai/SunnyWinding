#include <iostream>
#include "Assert.h"

// ���ԣ����һЩ��Ҫ��Ϣ
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