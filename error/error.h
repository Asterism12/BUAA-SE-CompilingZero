#pragma once
#include <string>
#include <iostream>
class Error {
private:
	std::string _errorMessage;
	int _lineNumber = 0;
public:
	Error(std::string errorMessage) :_errorMessage(errorMessage) {};
	Error(std::string errorMessage,int lineNumber) :_errorMessage(errorMessage),_lineNumber(lineNumber) {};
	void printErrorMessage();
};