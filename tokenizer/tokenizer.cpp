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

Token Tokenizer::NextToken() {
	//改用std::optional实现EOF
	if (!_initialized)
		readAll();
	if (_rdr.bad()) {
		throw Error("StreamError");
	}
	if (isEOF()) {
		throw Error("EOF");
	}
	return nextToken();
}

std::vector<Token> Tokenizer::AllTokens() {
	std::vector<Token> result;

	while (true) {
		auto p = NextToken();
		if (p.second.has_value()) {
			if (p.second.value().GetCode() == ErrorCode::ErrEOF)
				return std::make_pair(result, std::optional<CompilationError>());
			else
				return std::make_pair(std::vector<Token>(), p.second);
		}
		result.emplace_back(p.first.value());
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