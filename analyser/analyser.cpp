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
		if (!next.has_value()) {
			return;
		}
		if (!(next.value().GetType() == TokenType::RESERVED_WORD)) {
			return;
		}
		if (std::any_cast<std::string>(next.value().GetValue()) == "const") {
			char type = analyse_type_specifier();
			
			if (!analyse_init_declarator_list()) {
				throw Error("Missing < init - declarator - list>", next.value().GetLine());
			}
			switch (type)
			{
			case 'i':
				
			default:
				throw Error("Missing <type - specifier>", next.value().GetLine());
				break;
			}
		}
		else {
			unreadToken();
			char type = analyse_type_specifier();
			analyse_init_declarator_list();
			switch (type)
			{
			case 'i':

			default:
				throw Error("Missing <type - specifier>", next.value().GetLine());
				break;
			}
		}
	}
}

void Analyser::analyse_function_definition() {

}

char Analyser::analyse_type_specifier() {
	auto next = nextToken();
	if (!next.has_value()) {
		throw Error("Missing <type - specifier>", next.value().GetLine());
	}
	if (std::any_cast<std::string>(next.value().GetValue()) == "int") {
		return 'i';
	}
	else {
		throw Error("Wrong  <type - specifier>", next.value().GetLine());
	}
}

bool Analyser::analyse_init_declarator_list() {

}

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