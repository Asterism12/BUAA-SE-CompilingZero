#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <optional>
#include "token.h"
enum class DFAState {
	INITIAL_STATE,
	UNSIGNED_INTEGER_STATE,
	PLUS_SIGN_STATE,
	MINUS_SIGN_STATE,
	DIVISION_SIGN_STATE,
	MULTIPLICATION_SIGN_STATE,
	IDENTIFIER_STATE,
	EQUAL_SIGN_STATE,
	SEMICOLON_STATE,
	LEFTBRACKET_STATE,
	RIGHTBRACKET_STATE,
	GREATER_THAN_STATE,
	LESS_THAN_STATE
};

class Tokenizer {
public:
	Tokenizer(std::istream& ifs): _rdr(ifs), _initialized(false), _line(0) {};
	std::vector<Token> AllTokens();
	Token NextToken();
private:
	std::istream& _rdr;
	bool _initialized;
	std::vector<std::string> _lines_buffer;
	std::pair<uint64_t, uint64_t> _ptr;

	Token nextToken();
	void Tokenizer::readAll() {}
	std::pair<uint64_t, uint64_t> nextPos();

	std::optional<char> nextChar();
	bool isEOF();
	void unreadLast();
};