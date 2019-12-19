#include "analyser/analyser.h"

void Analyser::Analyse() {
	analyse_C0_sprogram();
	std::cout << "Analyser successful return." << std::endl;
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
				addVariable(std::any_cast<std::string>(next.value().GetValue()), true);
				bool has_initializer = analyse_initializer(type);
				if (!has_initializer) {
					throw Error("Missing <initializer>", _currentLine);
				}
				next = nextToken();
				if (!next.has_value()) {
					throw Error("Missing ';'", _currentLine);
				}
			} while (next.value().GetType() == TokenType::COMMA_SIGH);
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
				addVariable(std::any_cast<std::string>(next.value().GetValue()), true);
				bool has_initializer = analyse_initializer(type);
				if (!has_initializer) {
					initializeVariable(type);
				}
				next = nextToken();
				if (!next.has_value()) {
					throw Error("Missing ';'", _currentLine);
				}
			} while (next.value().GetType() == TokenType::COMMA_SIGH);
			if (next.value().GetType() != TokenType::SEMICOLON) {
				throw Error("Missing ';'", _currentLine);
			}
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
		switch (type)
		{
		case TokenType::PLUS_SIGN:
			analyse_multiplicative_expression();
			addInstruction(Instruction(Operation::iadd));
			break;
		case TokenType::MINUS_SIGN:
			analyse_multiplicative_expression();
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

//<multiplicative-expression> ::= 
//	<unary - expression>{<multiplicative - operator><unary - expression>}
void Analyser::analyse_multiplicative_expression() {
	analyse_unary_expression();
	auto next = nextToken();

	while (next.has_value()) {
		auto type = next.value().GetType();
		switch (type)
		{
		case TokenType::MULTIPLICATION_SIGN:
			analyse_unary_expression();
			addInstruction(Instruction(Operation::imul));
			break;
		case TokenType::DIVISION_SIGN:
			analyse_unary_expression();
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
	if (next.value().GetType() == TokenType::PLUS_SIGN) {
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
	std::int32_t index;
	switch (next.value().GetType())
	{
	case TokenType::LEFT_BRACKET:
		analyse_expression();
		next = nextToken();
		if (!next.has_value() || next.value().GetType() != TokenType::RIGHT_BRACKET) {
			throw Error("Missing ')'");
		}
		break;
	case TokenType::IDENTIFIER:
		if (!loadVariable(std::any_cast<std::string>(next.value().GetValue()))) {
			unreadToken();
			analyse_function_call();
		}
		break;
	case TokenType::UNSIGNED_INTEGER:
		index = addConstant(next.value());
		addInstruction(Instruction(Operation::loadc, index));
		break;
	default:
		throw Error("Missing <primary-expression>", _currentLine);
		break;
	}
	if (factor == -1) {
		addInstruction(Instruction(Operation::isub, 0));
	}
}


//<function-call> ::= 
//	<identifier> '('[<expression - list>] ')'
//<expression-list> ::= 
//	<expression>{',' < expression > }
void Analyser::analyse_function_call() {
	//<identifier>
	auto next = nextToken();
	if (!next.has_value() || next.value().GetType() != TokenType::IDENTIFIER) {
		throw Error("Missing identifier", _currentLine);
	}
	std::string func = std::any_cast<std::string>(next.value().GetValue());
	int index = getFunctionIndex(func);
	std::vector<char> para = getFunctionParameter(func);
	//'('
	next = nextToken();
	if (!next.has_value() || next.value().GetType() != TokenType::LEFT_BRACKET) {
		throw Error("Missing '('", _currentLine);
	}
	//<expression-list>
	for (int i = 0; i < para.size(); i++) {
		analyse_expression();
		if (i != para.size() - 1) {
			next = nextToken();
			if (!next.has_value() || next.value().GetType() != TokenType::COMMA_SIGH) {
				throw Error("Missing ','", _currentLine);
			}
		}
	}
	//')'
	next = nextToken();
	if (!next.has_value() || next.value().GetType() != TokenType::RIGHT_BRACKET) {
		throw Error("Missing ')'", _currentLine);
	}
	addInstruction(Instruction(Operation::call, index));
}

//<function-definition> ::= 
//	<type - specifier><identifier><parameter - clause><compound - statement>
void Analyser::analyse_function_definition() {
	char type = analyse_type_specifier();
	//<identifier>
	auto next = nextToken();
	if (!next.has_value() || next.value().GetType() != TokenType::IDENTIFIER) {
		throw Error("Missing identifier", _currentLine);
	}
	std::string func = std::any_cast<std::string>(next.value().GetValue());
	addFunction(func);
	addConstant(next.value());
	//<parameter - clause>
	analyse_parameter_clause();
	analyse_compound_statement();
}

//<parameter-clause> ::= 
//	'('[<parameter - declaration - list>] ')'
//< parameter - declaration - list > :: =
//	<parameter - declaration>{ ',' < parameter - declaration > }
//<parameter-declaration> ::= 
//	[<const - qualifier>]<type - specifier><identifier>
void Analyser::analyse_parameter_clause() {
	auto next = nextToken();
	if (!next.has_value() || next.value().GetType() != TokenType::LEFT_BRACE) {
		throw Error("Missing '('", _currentLine);
	}
	//<parameter-declaration>
	next = nextToken();
	while (true) {
		if (!next.has_value() || next.value().GetType() != TokenType::RESERVED_WORD) {
			break;
		}
		//const
		if (std::any_cast<std::string>(next.value().GetValue()) == "const") {
			char type = analyse_type_specifier();
				//<identifier>
				next = nextToken();
				if (!next.has_value() || (next.value().GetType() != TokenType::IDENTIFIER)) {
					throw Error("Missing <identifier>", _currentLine);
				}
				addVariable(std::any_cast<std::string>(next.value().GetValue()), true);
		}
		//non-const
		else {
			unreadToken();
			char type = analyse_type_specifier();
			next = nextToken();
			if (!next.has_value() || (next.value().GetType() != TokenType::IDENTIFIER)) {
				throw Error("Missing <identifier>", _currentLine);
			}
			addVariable(std::any_cast<std::string>(next.value().GetValue()), false);
		}
		//','
		next = nextToken();
		if (!next.has_value() || next.value().GetType() != TokenType::COMMA_SIGH) {
			break;
		}
		//预读判断是否为func(int a ,)这种错误情况
		next = nextToken();
		if (!next.has_value() || next.value().GetType() != TokenType::RESERVED_WORD) {
			throw Error("Missing <identifier>", _currentLine);
		}
	}
	if (!next.has_value() || next.value().GetType() != TokenType::SEMICOLON) {
		throw Error("Missing ';'", _currentLine);
	}
}

void Analyser::analyse_compound_statement() {
	
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

bool Analyser::loadVariable(const std::string& var) {
	std::optional<std::int32_t> index = getIndexInLocal(var);
	if (index.has_value()) {
		addInstruction(Instruction(Operation::loada, 0, index.value()));
		return true;
	}
	index = getIndexInGlobal(var);
	if (index.has_value()) {
		addInstruction(Instruction(Operation::loada, 1, index.value()));
		return true;
	}
	return false;
}

void Analyser::addVariable(const std::string& var, bool isConstant) {
	std::pair<bool, std::int32_t> valStruct(isConstant, _localIndex);
	if (_currentFunction == -1) {
		if (_globalVars.find(var) != _globalVars.end()) {
			throw Error("this identifier has been declared", _currentLine);
		}
		else {
			_globalVars[var] = valStruct;
		}
	}
	else {
		if (_localVars.find(var) != _localVars.end()) {
			throw Error("this identifier has been declared", _currentLine);
		}
		else {
			_localVars[var] = valStruct;
		}
	}
	_localIndex++;
}

std::optional<std::int32_t> Analyser::getIndexInLocal(const std::string& s) {
	if (_localVars.find(s) != _localVars.end()) {
		return _localVars[s].second;
	}
	else {
		return {};
	}
}

std::optional<std::int32_t> Analyser::getIndexInGlobal(const std::string& s) {
	if (_globalVars.find(s) != _globalVars.end()) {
		return _globalVars[s].second;
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

std::int32_t Analyser::addConstant(const Token &tk)
{
	//warning! There is no type check!
	_consts.push_back(tk.GetValue());
	return _consts.size() - 1;
}

void Analyser::addFunction(const std::string& func)
{
	if (_functions.find(func) != _functions.end()) {
		throw Error("function has been declared", _currentLine);
	}
	else if (_globalVars.find(func) != _globalVars.end()) {
		throw Error("this identifier has been declared", _currentLine);
	}
	else {
		//add & switch
		_functions[func] = _currentFunction;
		_currentFunction++;
		_localVars.clear();
		_localIndex = 0;
	}
}

int Analyser::getFunctionIndex(const std::string& s)
{
	if (_functions.find(s) != _functions.end()) {
		return _functions[s];
	}
	throw Error("function is not exist", _currentLine);
}

std::vector<char> Analyser::getFunctionParameter(const std::string& s)
{
	if (_functions.find(s) != _functions.end()) {
		return _functionParameter[s];
	}
	throw Error("function is not exist", _currentLine);
}

