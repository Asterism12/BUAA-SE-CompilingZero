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

				//在min0的基础上添加了6种状态,未完成：16进制
				case '>':
					current_state = DFAState::GREATER_THAN_STATE;
					break;
				case '<':
					current_state = DFAState::LESS_THAN_STATE;
					break;
				case '{':
					current_state = DFAState::LEFTBRACE_STATE;
					break;
				case '}':
					current_state = DFAState::RIGHTBRACE_STATE;
					break;
				case '!':
					current_state = DFAState::EXCLAMATION_SIGH_STATE;
					break;
				case ',':
					current_state = DFAState::COMMA_STATE;

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
				if (ss.str()[0] = '0') {
					throw Error("StartWithZeroErr",_ptr.first);
				}
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
		case DFAState::IDENTIFIER_STATE: {
			// 如果当前已经读到了文件尾，则解析已经读到的字符串
			//     如果解析结果是关键字，那么返回对应关键字的token，否则返回标识符的token
			// 如果读到的是字符或字母，则存储读到的字符
			// 如果读到的字符不是上述情况之一，则回退读到的字符，并解析已经读到的字符串
			//     如果解析结果是关键字，那么返回对应关键字的token，否则返回标识符的token

			if (!current_char.has_value()) {
				std::string str = ss.str();
				if (ReservedWords.find(str) != ReservedWords.end()) {
					return Token(TokenType::RESERVED_WORD, str);
				}
				else {
					return Token(TokenType::IDENTIFIER, str);
				}
			}
			auto ch = current_char.value();
			if (miniplc0::isalpha(ch) || miniplc0::isdigit(ch)) {
				ss << ch;
			}
			else {
				unreadLast();
				std::string str = ss.str();
				if (ReservedWords.find(str) != ReservedWords.end()) {
					return Token(TokenType::RESERVED_WORD, str);
				}
				else {
					return Token(TokenType::IDENTIFIER, str);
				}
			}
			break;
		}
		case DFAState::PLUS_SIGN_STATE: {
			unreadLast(); // Yes, we unread last char even if it's an EOF.
			return Token(TokenType::PLUS_SIGN,'+');
		}
		case DFAState::MINUS_SIGN_STATE: {
			unreadLast();
			return Token(TokenType::MINUS_SIGN, '-');		
		}
		case DFAState::MULTIPLICATION_SIGN_STATE: {
			unreadLast(); 
			return Token(TokenType::MULTIPLICATION_SIGN, '*');			
		}
		case DFAState::DIVISION_SIGN_STATE: {
			unreadLast(); 
			return Token(TokenType::DIVISION_SIGN, '/');
		}
		case DFAState::SEMICOLON_STATE: {
			unreadLast(); 
			return Token(TokenType::SEMICOLON, ';');
		}
		case DFAState::LEFTBRACKET_STATE: {
			unreadLast(); 
			return Token(TokenType::LEFT_BRACKET, '(');
		}
		case DFAState::RIGHTBRACKET_STATE: {
			unreadLast(); 
			return Token(TokenType::RIGHT_BRACKET, ')');
		}

		//c0文法
		case DFAState::COMMA_STATE: {
			unreadLast();
			return Token(TokenType::COMMA_SIGH, ',');
		}
		case DFAState::LEFTBRACE_STATE:{
			unreadLast();
			return Token(TokenType::LEFT_BRACE, '{');
		}
		case DFAState::RIGHTBRACE_STATE: {
			unreadLast();
			return Token(TokenType::RIGHT_BRACE, '}');
		}
		case DFAState::EQUAL_SIGN_STATE: {
			//= & ==
			auto ch = current_char.value();
			if (ch == '=') {
				return Token(TokenType::EQUAL_SIGN, "==");
			}
			else {
				unreadLast();
				return Token(TokenType::ASSIGNMENT_SIGN, '=');
			}
		}
		case DFAState::GREATER_THAN_STATE: {
			//> & >=
			auto ch = current_char.value();
			if (ch == '=') {
				return Token(TokenType::GREATER_THAN_EQUAL_SIGH,">=");
			}
			else {
				unreadLast();
				return Token(TokenType::GREATER_THAN_SIGH, '{');
			}
		}
		case DFAState::LESS_THAN_STATE: {
			//< & <=
			auto ch = current_char.value();
			if (ch == '=') {
				return Token(TokenType::LESS_THAN_EQUAL_SIGH, "<=");
			}
			else {
				unreadLast();
				return Token(TokenType::LESS_THAN_SIGH, '<');
			}
		}
		case DFAState::EXCLAMATION_SIGH_STATE: {
			//!=
			auto ch = current_char.value();
			if (ch == '=') {
				return Token(TokenType::UNEQUAL_SIGN, "!=");
			}
			else {
				throw Error("InvalidInputErr");
			}
		}
		default:
			break;
		}
	}
}