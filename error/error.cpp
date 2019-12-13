#pragma once
#include "error.h"

void Error::printErrorMessage() {
	std::cout << "Error : " + _errorMessage << std::endl;
}