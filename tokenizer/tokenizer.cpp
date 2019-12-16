#include "tokenizer.h"
#include "../error/error.h"
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
		}
		default:
			break;
		}
	}
}