#pragma once
#include <string>
#include <iostream>
class Error {
private:
	std::string _errorMessage;
	std::uint64_t _lineNumber = 0;
public:
	Error(std::string errorMessage) :_errorMessage(errorMessage) {};
	Error(std::string errorMessage,std::uint64_t lineNumber) :_errorMessage(errorMessage),_lineNumber(lineNumber) {};
	void printErrorMessage();
};