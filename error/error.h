#pragma once
#include <string>
#include <iostream>
enum class ErrorType {
	ParameterError,
	TokenError,
	AnalyseError,
	OhterError
};
class Error {
private:
	std::string _errorMessage;
	ErrorType _errorType;
public:
	Error(std::string errorMessage,ErrorType errorType) :_errorMessage(errorMessage),_errorType(errorType) {};
	ErrorType errorType();
	void printErrorMessage();
};