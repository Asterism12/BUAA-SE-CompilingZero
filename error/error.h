#pragma once
#include <string>
#include <iostream>
class Error {
private:
	std::string _errorMessage;
public:
	Error(std::string errorMessage) :_errorMessage(errorMessage) {};
	void printErrorMessage();
};