#pragma once
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

namespace c0 {
	std::set<std::string> ReservedWords = {
	"void",
	"const",
	"int",
	"double",
	"char",
	"struct",
	"if",
	"else",
	"switch",
	"case",
	"default",
	"while",
	"for",
	"do",
	"return",
	"break",
	"continue",
	"print",
	"scan"
	};
}

class Token final {
public:
	Token(TokenType type, std::any value) :_type(type), _value(value) {};
	TokenType GetType() const { return _type; };
	std::any GetValue() const { return _value; };
private:
	TokenType _type;
	std::any _value;
};