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
					initializeVariable(type);
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
		throw Error("Missing <type - specifier>", _currentLine);
	}
	if (std::any_cast<std::string>(next.value().GetValue()) == "int") {
		return 'i';
	}
	else {
		throw Error("Wrong identifier type", _currentLine);
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

//<expression> ::= 
//	<additive - expression>
//<additive - expression> :: =
//	<multiplicative - expression>{ <additive - operator><multiplicative - expression> }
void Analyser::analyse_expression() {
	analyse_multiplicative_expression();
	auto next = nextToken();

	while (next.has_value()) {
		auto type = next.value().GetType();
		analyse_multiplicative_expression();
		switch (type)
		{
		case TokenType::PLUS_SIGN:
			addInstruction(Instruction(Operation::iadd));
			break;
		case TokenType::MINUS_SIGN:
			addInstruction(Instruction(Operation::isub));
			break;
		default:
			unreadToken();
			return;
		}
		next = nextToken();
	}
	return;
}

void Analyser::analyse_multiplicative_expression() {
	analyse_unary_expression();
	auto next = nextToken();

	while (next.has_value()) {
		auto type = next.value().GetType();
		analyse_unary_expression();
		switch (type)
		{
		case TokenType::MULTIPLICATION_SIGN:
			addInstruction(Instruction(Operation::imul));
			break;
		case TokenType::DIVISION_SIGN:
			addInstruction(Instruction(Operation::idiv));
			break;
		default:
			unreadToken();
			return;
		}
		next = nextToken();
	}
	return;
}

//<unary - expression> :: =
//	[<unary - operator>]<primary - expression>
//<primary-expression> ::=  
//	'(' < expression > ')'
//	| <identifier>
//	| <integer - literal>
//	| <function - call>
void Analyser::analyse_unary_expression() {
	auto next = nextToken();
	if (!next.has_value()) {
		throw Error("Missing <expression>", _currentLine);
	}
	//<unary - operator>
	std::int32_t factor = 1;
	if (next.value().GetType == TokenType::PLUS_SIGN) {
		next = nextToken();
	}
	else if (next.value().GetType() == TokenType::MINUS_SIGN) {
		factor = -1;
		addInstruction(Instruction(Operation::ipush, 0));
		next = nextToken();
	}
	//primary - expression
	if (!next.has_value()) {
		throw Error("Missing <primary-expression>", _currentLine);
	}
	switch (next.value().GetType())
	{
	case TokenType::LEFT_BRACKET:
		analyse_expression();
		next = nextToken();
		if (!next.has_value() || next.value().GetType != TokenType::RIGHT_BRACKET) {
			throw Error("Missing ')'");
		}
		break;
	case TokenType::IDENTIFIER:
		loadVariable(next.value());
		//函数调用
		break;
	case TokenType::UNSIGNED_INTEGER:
		std::int32_t index = addConstant(next.value());
		addInstruction(Instruction(Operation::loadc, index));
		break;
	default:
		throw Error("Missing <primary-expression>");
	}
	if (factor = -1) {
		addInstruction(Instruction(Operation::isub, 0));
	}
}

void Analyser::analyse_function_definition() {

}

std::optional<Token> Analyser::nextToken() {
	if (_offset == _tokens.size())
		return {};
	_currentLine = _tokens[_offset].GetLine();
	return _tokens[_offset++];
}

void Analyser::unreadToken() {
	if (_offset == 0) {
		throw Error("analyser unreads token from the begining", _currentLine);
	}
	_currentLine = _tokens[_offset - 1].GetLine();
	_offset--;
}

void Analyser::initializeVariable(char type) {
	switch (type)
	{
	case 'i':
		addInstruction(Instruction(Operation::ipush, 0));
		break;
	default:
		break;
	}
}

void Analyser::loadVariable(const Token& tk) {
	if (tk.GetType() != TokenType::IDENTIFIER) {
		throw Error("only identifier can be load", _currentLine);
	}
	std::string var = std::any_cast<std::string>(tk.GetValue());
	std::optional<std::int32_t> index = getIndexInLocal(var);
	if (index.has_value()) {
		addInstruction(Instruction(Operation::loada, 0, index.value()));
	}
	else {
		index = getIndexInGlobal(var);
		if (index.has_value()) {
			addInstruction(Instruction(Operation::loada, 1, index.value()));
		}
		else {
			throw Error("can not find the identifier", _currentLine);
		}
	}
}

void Analyser::addVariable(const Token& tk) {
	if (tk.GetType() != TokenType::IDENTIFIER) {
		throw Error("only identifier can be added to the table");
	}
	std::string var = std::any_cast<std::string>(tk.GetValue());
	if (_currentFunction == -1) {
		if (_globalVars.find(var) != _globalVars.end()) {
			throw Error("this identifier has been declared", _currentLine);
		}
		else {
			_globalVars[var] = _localIndex;
		}
	}
	else {
		if (_localVars.find(var) != _localVars.end()) {
			throw Error("this identifier has been declared", _currentLine);
		}
		else {
			_localVars[var] = _localIndex;
		}
	}
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

void Analyser::addInstruction(Instruction instruction) {
	if (_currentFunction == -1) {
		_startInstructions.push_back(instruction);
	}
	else {
		_instructions[_currentFunction].push_back(instruction);
	}
}

//返回常量index
std::int32_t Analyser::addConstant(const Token& tk)
{
	//warning! There is no type check!
	_consts.push_back(tk.GetValue());
	return _consts.size() - 1;
}
