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

	//�������������������Щ���ݽṹ��
	//������
	std::vector<std::any> _consts;
	//ȫ�ֱ���������-ƫ��
	std::map<std::string, std::int32_t> _globalVars;
	//start_code
	std::vector<Instruction> _startInstructions;
	//����������-�������
	std::map<std::string, std::int32_t> _functions;
	//������Ӧ��ָ����У��������-ָ�����
	std::map<std::int32_t, std::vector<Instruction>> _instructions;
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

	//token���
	std::optional<Token> nextToken();
	void unreadToken();
	std::vector<Token> _tokens;
	std::size_t _offset;

	//�ֲ�����������-ƫ��
	std::map<std::string, std::int32_t> _localVars;
	std::optional<std::int32_t> getIndexInLocal(const std::string&);
	std::int32_t _localIndex;
	std::int32_t _currentFunction;
	std::int32_t _currentLine;
	void addVariable(const Token&);
	void initializeVariable(char type);

	//ȫ�ָ�������������
	std::optional<std::int32_t> getIndexInGlobal(const std::string&);
	std::int32_t _globalIndex;
	void addInstruction(Instruction instruction);
	void loadVariable(const Token&);
};