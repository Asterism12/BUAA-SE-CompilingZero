#include "analyser/analyser.h"

void Analyser::Analyse() {
	analyse_C0_sprogram();
}
//<C0-program> ::= 
//	{<variable - declaration>} {<function - definition>}
void Analyser::analyse_C0_sprogram() {
	analyse_variable_declaration();
	analyse_function_definition();
}

//<variable-declaration> ::= 
//	[<const - qualifier>]<type - specifier> < init - declarator - list>';'
void Analyser::analyse_variable_declaration() {
	while (true) {
		auto next = nextToken();
		if (!next.has_value() || (next.value().GetType() != TokenType::RESERVED_WORD)) {
			return;
		}
		//const
		if (std::any_cast<std::string>(next.value().GetValue()) == "const") {
			char type = analyse_type_specifier();
			do {
				//init - declarator - list
				next = nextToken();
				if (!next.has_value() || (next.value().GetType() != TokenType::IDENTIFIER)) {
					throw Error("Missing < init - declarator - list>", _currentLine);
				}
				addVariable(next.value());
				bool has_initializer = analyse_initializer(type);
				if (!has_initializer) {
					throw Error("Missing <initializer>", _currentLine);
				}
				next = nextToken();
				if (!next.has_value()) {
					throw Error("Missing ';'", _currentLine);
				}
			} while (next.value().GetType() != TokenType::COMMA_SIGH);
			if (next.value().GetType() != TokenType::SEMICOLON) {
				throw Error("Missing ';'", _currentLine);
			}
			return;
		}
		//non-const
		else {
			unreadToken();
			char type = analyse_type_specifier();
			do {
				////init - declarator - list
				next = nextToken();
				if (!next.has_value() || (next.value().GetType() != TokenType::IDENTIFIER)) {
					throw Error("Missing < init - declarator - list>", _currentLine);
				}
				addVariable(next.value());
				bool has_initializer = analyse_initializer(type);
				if (!has_initializer) {
					initializeVar(type);
				}
				next = nextToken();
				if (!next.has_value()) {
					throw Error("Missing ';'", _currentLine);
				}
			} while (next.value().GetType() != TokenType::COMMA_SIGH);
			if (next.value().GetType() != TokenType::SEMICOLON) {
				throw Error("Missing ';'", _currentLine);
			}
			return;
		}
	}
}

//<type-specifier> ::= 
//	<simple-type-specifier>
char Analyser::analyse_type_specifier() {
	auto next = nextToken();
	if (!next.has_value()) {
		throw Error("Missing <type - specifier>", next.value().GetLine());
	}
	if (std::any_cast<std::string>(next.value().GetValue()) == "int") {
		return 'i';
	}
	else {
		throw Error("Wrong identifier type", next.value().GetLine());
	}
}

//<initializer> ::= 
//	'=' < expression >
bool Analyser::analyse_initializer(char type) {
	auto next = nextToken();
	if (!next.has_value() || (next.value().GetType() != TokenType::ASSIGNMENT_SIGN)) {
		return false;
	}
	analyse_expression();
	return true;
}

void Analyser::initializeVar(char type) {
	switch (type)
	{
	case 'i':
		_instructions[_currentFunction].push_back(Instruction(Operation::ipush, 0));
		break;
	default:
		break;
	}
}

void Analyser::analyse_expression() {
	analyse_multiplicative_expression();
	auto next = nextToken();

	while (next.has_value()) {
		auto type = next.value().GetType();
		analyse_multiplicative_expression();
		switch (type)
		{
		case TokenType::PLUS_SIGN:
			_instructions[_currentFunction].push_back(Instruction(Operation::iadd));
			break;
		case TokenType::MINUS_SIGN:
			_instructions[_currentFunction].push_back(Instruction(Operation::isub));
			break;
		default:
			return;
		}
	}
	return;
}

void Analyser::analyse_multiplicative_expression() {

}

void Analyser::analyse_function_definition() {

}

std::optional<Token> Analyser::nextToken() {
	if (_offset == _tokens.size())
		return {};
	_currentLine = _tokens[_offset++].GetLine();
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