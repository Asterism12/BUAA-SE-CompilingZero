#include "error.h"

ErrorType Error::errorType() {
	return _errorType;
}

void Error::printErrorMessage() {
	std::cout << "Error : "+_errorMessage << std::endl;
}