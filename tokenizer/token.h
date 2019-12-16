#pragma once
class enum TokenType {
	NULL_TOKEN,
	UNSIGNED_INTEGER,
	IDENTIFIER,
	BEGIN,
	END,
	VAR,
	CONST,
	PRINT,
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
	LESS_THAN_EQUAL_SIGH
};

class Token final{
public:
	TokenType GetType() const { return _type; };
	std::any GetValue() const { return _value; };
private:
	TokenType _type;
	std::any _value;
};