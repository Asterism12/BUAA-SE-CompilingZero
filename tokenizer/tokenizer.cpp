#include "tokenizer/tokenizer.h"

std::uint32_t htoi(std::string h) {
	if (h.size() <= 2 || h[0] != '0' || (h[1] != 'x' && h[1] != 'X')) {
		throw std::invalid_argument("received invalid argument");
	}
	std::uint64_t x = 1, ret = 0;
	for (int i = h.size() - 1; i >= 2; i--) {
		ret += h[i] * x;
		if (ret > INT_MAX) {
			throw std::out_of_range("out of range");
		}
	}
	return ret;
}

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
			//�����ļ�β
			if (!current_char.has_value()) {
				return {};
			}

			auto ch = current_char.value();
			// ����Ƿ�����˲��Ϸ����ַ�����ʼ��Ϊ��
			auto invalid = false;

			if (miniplc0::isspace(ch)) // �������ַ��ǿհ��ַ����ո񡢻��С��Ʊ���ȣ�
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

				//��min0�Ļ����������6��״̬
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

				// �����ܵ��ַ����µĲ��Ϸ���״̬
				default:
					invalid = true;
					break;
				}
				if (invalid) {
					unreadLast();
					throw Error("InvalidInputErr", _ptr.first + 1);
				}
				// ����������ַ�������״̬��ת�ƣ�˵������һ��token�ĵ�һ���ַ�
				if (current_state != DFAState::INITIAL_STATE) // ignore white spaces
					ss << ch; // �洢�������ַ�
				break;
			}
		}
		case DFAState::UNSIGNED_INTEGER_STATE: {
			// �����ǰ�Ѿ��������ļ�β��������Ѿ��������ַ���Ϊ����
			//     �����ɹ��򷵻��޷����������͵�token�����򷵻ر������
			// ����������ַ������֣���洢�������ַ�
			// �������������ĸ������Ƿ�Ϊx�Լ�����λ�÷���ʮ����������תΪʮ��������״̬������������
			// ����������ַ������������֮һ������˶������ַ����������Ѿ��������ַ���Ϊ����
			//     �����ɹ��򷵻��޷����������͵�token�����򷵻ر������
			if (!current_char.has_value()) {
				if (ss.str()[0] = '0') {
					throw Error("StartWithZeroErr", _ptr.first + 1);
				}
				try {
					int ret = std::stoi(ss.str());
					return Token(TokenType::UNSIGNED_INTEGER, ret);
				}
				catch (std::invalid_argument) {
					throw Error("InvalidAgumentErr", _ptr.first + 1);
				}
				catch (std::out_of_range) {
					throw Error("IntegerOverflowErr", _ptr.first + 1);
				}
			}
			auto ch = current_char.value();
			if (miniplc0::isdigit(ch)) {
				ss << ch;
			}
			else if (miniplc0::isalpha(ch)) {
				ss << ch;
				if (ss.str().size() == 2 && (ch == 'x' || ch == 'X')) {
					current_state = DFAState::HEXADECIMAL_STATE;
				}
				else {
					throw Error("InvalidInputErr", _ptr.first + 1);
				}
			}
			else {
				unreadLast();
				try {
					int ret = std::stoi(ss.str());
					return Token(TokenType::UNSIGNED_INTEGER, ret);
				}
				catch (std::invalid_argument) {
					throw Error("InvalidAgumentErr", _ptr.first + 1);
				}
				catch (std::out_of_range) {
					throw Error("IntegerOverflowErr", _ptr.first + 1);
				}
			}
			break;
		}
		case DFAState::HEXADECIMAL_STATE: {
			if (!current_char.has_value()) {
				try {
					int ret = htoi(ss.str());
					return Token(TokenType::UNSIGNED_INTEGER, ret);
				}
				catch (std::invalid_argument) {
					throw Error("InvalidAgumentErr", _ptr.first + 1);
				}
				catch (std::out_of_range) {
					throw Error("IntegerOverflowErr", _ptr.first + 1);
				}
			}
			auto ch = current_char.value();
			if (miniplc0::ishexadecimal(ch)) {
				ss << ch;
			}
			else {
				unreadLast();
				try {
					int ret = htoi(ss.str());
					return Token(TokenType::UNSIGNED_INTEGER, ret);
				}
				catch (std::invalid_argument) {
					throw Error("InvalidAgumentErr", _ptr.first + 1);
				}
				catch (std::out_of_range) {
					throw Error("IntegerOverflowErr", _ptr.first + 1);
				}
			}
			break;
		}
		case DFAState::IDENTIFIER_STATE: {
			// �����ǰ�Ѿ��������ļ�β��������Ѿ��������ַ���
			//     �����������ǹؼ��֣���ô���ض�Ӧ�ؼ��ֵ�token�����򷵻ر�ʶ����token
			// ������������ַ�����ĸ����洢�������ַ�
			// ����������ַ������������֮һ������˶������ַ����������Ѿ��������ַ���
			//     �����������ǹؼ��֣���ô���ض�Ӧ�ؼ��ֵ�token�����򷵻ر�ʶ����token

			if (!current_char.has_value()) {
				std::string str = ss.str();
				if (c0::ReservedWords.find(str) != c0::ReservedWords.end()) {
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
				if (c0::ReservedWords.find(str) != c0::ReservedWords.end()) {
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

		//c0�ķ�
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
				throw Error("InvalidInputErr", _ptr.first + 1);
			}
		}
		default:
			break;
		}
	}
}