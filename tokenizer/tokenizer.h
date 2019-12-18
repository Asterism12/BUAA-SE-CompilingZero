#pragma once
#include "tokenizer/token.h"
#include "tokenizer/utils.hpp"
#include "error/error.h"

#include <fstream>
#include <vector>
#include <string>
#include <optional>
#include <sstream>

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
	LESS_THAN_STATE,
	LEFTBRACE_STATE,
	RIGHTBRACE_STATE,
	EXCLAMATION_SIGH_STATE,
	COMMA_STATE,
	HEXADECIMAL_STATE
};

class Tokenizer {
public:
	Tokenizer(std::istream& ifs)
		: _rdr(ifs), _initialized(false), _ptr(0, 0), _lines_buffer() {}
	std::vector<Token> AllTokens();
	std::optional<Token> NextToken();
private:
	std::istream& _rdr;
	bool _initialized;
	std::vector<std::string> _lines_buffer;
	std::pair<std::uint64_t, std::uint64_t> _ptr;

	std::optional<Token> nextToken();
	void Tokenizer::readAll();

	std::pair<std::uint64_t, std::uint64_t> nextPos();
	std::pair<std::uint64_t, std::uint64_t> Tokenizer::previousPos();

	std::optional<char> nextChar();
	bool isEOF();
	void unreadLast();
};