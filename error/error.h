#pragma once
#include<string>
class Error {
private:
	std::string _errorMessage;
public:
	Error(std::string errorMessage) :_errorMessage(errorMessage) {};
	std::string errorMessage();
};