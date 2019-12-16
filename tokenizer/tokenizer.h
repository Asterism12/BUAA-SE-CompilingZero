#pragma once
#include <fstream>
#include <vector>
#include <string>
class enum DFAState {
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

private:
	std::istream& _rdr;
	std::vector<std::string> _lines_buffer;
	std::uint64_t _line;

	void Tokenizer::readAll() {}
};