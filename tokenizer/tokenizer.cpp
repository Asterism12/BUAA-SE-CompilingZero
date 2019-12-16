#include "tokenizer.h"
#include "../error/error.h"
#include "utils.hpp"
void Tokenizer::readAll() {
	if (_initialized)
		return;
	for (std::string tp; std::getline(_rdr, tp);)
		_lines_buffer.emplace_back(std::move(tp + "\n"));
	_initialized = true;
	_ptr = std::make_pair<int64_t, int64_t>(0, 0);
	return;
}

std::optional<Token> Tokenizer::NextToken() {
	if (!_initialized)
		readAll();
	if (_rdr.bad()) {
		throw Error("StreamError");
	}
	if (isEOF()) {
		return {};
	}
	return nextToken();
}

std::vector<Token> Tokenizer::AllTokens() {
	std::vector<Token> result;
	while (true) {
		auto p = NextToken();
		if (p.has_value()) {
			result.emplace_back(p.value());
		}
		return result;
	}
}

bool Tokenizer::isEOF() {
	return _ptr.first >= _lines_buffer.size();
}

std::optional<char> Tokenizer::nextChar() {
	if (isEOF())
		return {}; // EOF
	auto result = _lines_buffer[_ptr.first][_ptr.second];
	_ptr = nextPos();
	return result;
}

std::pair<uint64_t, uint64_t> Tokenizer::nextPos() {
	if (_ptr.first >= _lines_buffer.size())
		throw Error("advance after EOF");
	if (_ptr.second == _lines_buffer[_ptr.first].size() - 1)
		return std::make_pair(_ptr.first + 1, 0);
	else
		return std::make_pair(_ptr.first, _ptr.second + 1);
}

void Tokenizer::unreadLast() {
	_ptr = previousPos();
}

std::pair<uint64_t, uint64_t> Tokenizer::previousPos() {
	if (_ptr.first == 0 && _ptr.second == 0)
		throw Error("previous position from beginning");
	if (_ptr.second == 0)
		return std::make_pair(_ptr.first - 1, _lines_buffer[_ptr.first - 1].size() - 1);
	else
		return std::make_pair(_ptr.first, _ptr.second - 1);
}

std::optional<Token> Tokenizer::nextToken() {
	std::stringstream ss;
	DFAState current_state = DFAState::INITIAL_STATE;
	while (true) {
		auto current_char = nextChar();
		switch (current_state)
		{
		case DFAState::INITIAL_STATE: {
			//遭遇文件尾
			if (!current_char.has_value()) {
				return {};
			}

			auto ch = current_char.value();
			// 标记是否读到了不合法的字符，初始化为否
			auto invalid = false;

			if (miniplc0::isspace(ch)) // 读到的字符是空白字符（空格、换行、制表符等）
				current_state = DFAState::INITIAL_STATE; 
			else if (!miniplc0::isprint(ch)) // control codes and backspace
				invalid = true;
			else if (miniplc0::isdigit(ch))
				current_state = DFAState::UNSIGNED_INTEGER_STATE;
			else if (miniplc0::isalpha(ch))
				current_state = DFAState::IDENTIFIER_STATE;
			else {
				switch (ch) {
				case '=': 
					current_state = DFAState::EQUAL_SIGN_STATE;
					break;
				case '-':
					current_state = DFAState::MINUS_SIGN_STATE;
					break;
				case '+':
					current_state = DFAState::PLUS_SIGN_STATE;
					break;
				case '*':
					current_state = DFAState::MULTIPLICATION_SIGN_STATE;
					break;
				case '/':
					current_state = DFAState::DIVISION_SIGN_STATE;
					break;
				case ';':
					current_state = DFAState::SEMICOLON_STATE;
					break;
				case '(':
					current_state = DFAState::LEFTBRACKET_STATE;
					break;
				case ')':
					current_state = DFAState::RIGHTBRACKET_STATE;
					break;

				//在min0的基础上添加了2种状态
				case '>':
					current_state = DFAState::GREATER_THAN_STATE;
					break;
				case '<':
					current_state = DFAState::LESS_THAN_STATE;
					break;

					// 不接受的字符导致的不合法的状态
				default:
					invalid = true;
					break;
				}
				if (invalid) {
					unreadLast();
					throw Error("InvalidInputErr");
				}
				// 如果读到的字符导致了状态的转移，说明它是一个token的第一个字符
				if (current_state != DFAState::INITIAL_STATE) // ignore white spaces
					ss << ch; // 存储读到的字符
				break;
			}
		}
		case DFAState::UNSIGNED_INTEGER_STATE: {
			// 如果当前已经读到了文件尾，则解析已经读到的字符串为整数
			//     解析成功则返回无符号整数类型的token，否则返回编译错误
			// 如果读到的字符是数字，则存储读到的字符
			// 如果读到的是字母，则存储读到的字符，并切换状态到标识符
			// 如果读到的字符不是上述情况之一，则回退读到的字符，并解析已经读到的字符串为整数
			//     解析成功则返回无符号整数类型的token，否则返回编译错误
			if (!current_char.has_value()) {
				try {
					int ret = std::stoi(ss.str());
					return Token(TokenType::UNSIGNED_INTEGER, ret);
				}
				catch (std::invalid_argument) {
					throw Error("InvalidAgumentErr");
				}
				catch (std::out_of_range) {
					throw Error("IntegerOverflowErr");
				}
			}
			auto ch = current_char.value();
			if (miniplc0::isdigit(ch)) {
				ss << ch;
			}
			else if (miniplc0::isalpha(ch)) {
				ss << ch;
				current_state = DFAState::IDENTIFIER_STATE;
			}
			else {
				unreadLast();
				try {
					int ret = std::stoi(ss.str());
					return Token(TokenType::UNSIGNED_INTEGER, ret);
				}
				catch (std::invalid_argument) {
					throw Error("InvalidAgumentErr");
				}
				catch (std::out_of_range) {
					throw Error("IntegerOverflowErr");
				}
			}
			break;
		}
		default:
			break;
		}
	}
}