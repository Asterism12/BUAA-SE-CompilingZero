#pragma once
#include "error.h"

void Error::printErrorMessage() {
	if (_lineNumber == 0) {
		std::cout << "Error : " + _errorMessage + " ." << std::endl;
	}
	else {
		std::cout << "Error : " + _errorMessage + "in line" << _lineNumber + " ." << std::endl;
	}
}