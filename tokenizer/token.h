#pragma once
#include "error/error.h"

#include <any>
#include <set>
#include <string>
enum class TokenType {
	NULL_TOKEN,
	UNSIGNED_INTEGER,
	IDENTIFIER,
	RESERVED_WORD,
	PLUS_SIGN,
	MINUS_SIGN,
	MULTIPLICATION_SIGN,
	DIVISION_SIGN,
	EQUAL_SIGN,
	SEMICOLON,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	GREATER_THAN_SIGH,
	LESS_THAN_SIGH,
	GREATER_THAN_EQUAL_SIGH,
	LESS_THAN_EQUAL_SIGH,
	LEFT_BRACE,
	RIGHT_BRACE,
	EXCLAMATION_SIGH,
	UNEQUAL_SIGN,
	ASSIGNMENT_SIGN,
	COMMA_SIGH,
	HEXADECIMAL_SIGH
};

class Token final {
public:
	Token(TokenType type, std::any value, uint64_t lineNumber) 
		:_type(type), _value(value), _lineNumber(lineNumber) {};
	TokenType GetType() const { return _type; };
	std::any GetValue() const { return _value; };
	uint64_t GetLine() const { return _lineNumber; };
	/*
	std::string GetValueString() const {
		try {
			return std::any_cast<std::string>(_value);
		}
		catch (const std::bad_any_cast&) {}
		try {
			return std::string(1, std::any_cast<char>(_value));
		}
		catch (const std::bad_any_cast&) {}
		try {
			return std::to_string(std::any_cast<int32_t>(_value));
		}
		catch (const std::bad_any_cast&) {
			Error("No suitable cast for token value.");
		}
		return "Invalid";
	}
	*/
private:
	TokenType _type;
	std::any _value;
	uint64_t _lineNumber;
};