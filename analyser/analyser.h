#pragma once
#include "tokenizer/token.h"
#include "instruction/instruction.h"

#include <any>
#include <cstdint>
#include <optional>
#include <vector>
#include <map>
class Analyser final {
public:
	Analyser(std::vector<Token> v)
		: _tokens(std::move(v)), _offset(0), _instructions({}) {}

	// Ψһ�ӿ�
	void Analyse();

	//�������������������Щ���ݽṹ��
	//������
	std::vector<std::any> _consts;
	//ȫ�ֱ�����
	std::vector<std::string> _globalVars;
	//����������-�������
	std::map<std::string, std::int32_t> _functions;
	//������Ӧ��ָ����У��������-ָ�����
	std::map<std::int32_t, std::vector<Instruction>> _instructions;
private:
	// ���еĵݹ��ӳ���


	std::optional<Token> nextToken();
	void unreadToken();
	void addVariable(const Token&);
	int32_t getIndex(const std::string&);

	std::vector<Token> _tokens;
	std::size_t _offset;

	//�ֲ�������
	std::vector<std::string> _localVars;
};