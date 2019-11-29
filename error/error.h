#pragma once
#ifndef _ERROR_H_
#define _ERROR_H_
#include<string>
class Error {
private:
	std::string _errorMessage;
public:
	Error(std::string errorMessage) :_errorMessage;
	std::string errorMessage();
};
#endif // !_ERROR_H_