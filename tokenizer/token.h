#pragma once
#include <any>
enum class TokenType {
	NULL_TOKEN,
	UNSIGNED_INTEGER,
	IDENTIFIER,
	VOID,//reserved-word
	CONST,
	INT,
	DOUBLE,
	CHAR,
	STRUCT,
	IF,
	ELSE,
	SWITCH,
	CASE,
	DEFAULT,
	WHILE,
	FOR,
	DO,
	RETURN,
	BREAK,
	CONTINUE,
	PRINT,
	SCAN,
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
	LEFTBRACE_STATE,
	RIGHTBRACE_STATE,
	EXCLAMATION_SIGH_STATE
};

class Token final{
public:
	Token(TokenType type, std::any value) :_type(type), _value(value) {};
	TokenType GetType() const { return _type; };
	std::any GetValue() const { return _value; };
private:
	TokenType _type;
	std::any _value;
};