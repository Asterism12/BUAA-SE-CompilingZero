#pragma once
#include "tokenizer/token.h"
#include "instruction/instruction.h"

#include <any>
#include <cstdint>
#include <optional>
#include <vector>
#include <map>
#include <string>
class Analyser final {
public:
	Analyser(std::vector<Token> v)
		: _tokens(std::move(v)), _offset(0), _globalIndex(0),
		_currentFunction(-1), _localIndex(0), _currentLine(0) {};

	// Ψһ�ӿ�
	void Analyse();

	//debug interface
	std::vector<std::string> endState();

	//�������������������Щ���ݽṹ��
	//������
	std::vector<std::any> _consts;
	//start_code
	std::vector<Instruction> _startInstructions;
	//������Ӧ��ָ����У��������-ָ�����
	std::vector<std::vector<Instruction>> _instructions;
private:
	// ���еĵݹ��ӳ���
	void analyse_C0_sprogram();
	void analyse_variable_declaration();
	void analyse_function_definition();
	char analyse_type_specifier();
	bool analyse_initializer(char type);
	void analyse_expression();
	void analyse_multiplicative_expression();
	void analyse_unary_expression();
	void analyse_function_call();

	//token���
	std::optional<Token> nextToken();
	void unreadToken();
	std::vector<Token> _tokens;
	std::size_t _offset;
	std::int32_t _currentLine;

	//�ֲ�����������-<�Ƿ��ǳ���,ƫ��>
	std::map<std::string, std::pair<bool, std::int32_t>> _localVars;
	std::optional<std::int32_t> getIndexInLocal(const std::string&);
	std::int32_t _localIndex;
	

	//ȫ�ֱ���������-<�Ƿ��ǳ���,ƫ��>
	std::map<std::string, std::pair<bool, std::int32_t>> _globalVars;
	std::optional<std::int32_t> getIndexInGlobal(const std::string&);
	std::int32_t _globalIndex;

	//�������
	std::int32_t addConstant(const Token&);
	void addInstruction(Instruction instruction);
	void addVariable(const Token&, bool isConstant);
	bool loadVariable(const Token&);
	void initializeVariable(char type);
	
	//�������
	std::map<std::string, std::int32_t> _functions;
	std::map<std::string, std::vector<char>> _functionParameter;
	std::int32_t _currentFunction;
	int getFunctionIndex(const std::string&);
	std::vector<char> getFunctionParameter(const std::string&);
};