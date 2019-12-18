#include "analyser/analyser.h"
std::optional<Token> Analyser::nextToken() {
	if (_offset == _tokens.size())
		return {};
	return _tokens[_offset++];
}

void Analyser::unreadToken() {
	if (_offset == 0)
		Error("analyser unreads token from the begining.");
	_offset--;
}

void Analyser::addVariable(const Token& tk) {
	if (tk.GetType() != TokenType::IDENTIFIER)
		Error("only identifier can be added to the table.");
	_localVars[std::any_cast<std::string>(tk.GetValue())] = _localIndex;
	_localIndex++;
}

std::optional<std::int32_t> Analyser::getIndexInLocal(const std::string& s) {
	if (_localVars.find(s) != _localVars.end()) {
		return _localVars[s];
	}
	else {
		return {};
	}
}

std::optional<std::int32_t> Analyser::getIndexInGlobal(const std::string& s) {
	if (_globalVars.find(s) != _globalVars.end()) {
		return _globalVars[s];
	}
	else {
		return {};
	}
}

void Analyser::Analyse() {

}