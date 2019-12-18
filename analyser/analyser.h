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
		: _tokens(std::move(v)), _offset(0), _instructions({}), _globalIndex(0){}

	// Ψһ�ӿ�
	void Analyse();

	//�������������������Щ���ݽṹ��
	//������
	std::vector<std::any> _consts;
	//ȫ�ֱ���������-ƫ��
	std::map<std::string, std::int32_t> _globalVars;
	//����������-�������
	std::map<std::string, std::int32_t> _functions;
	//������Ӧ��ָ����У��������-ָ�����
	std::map<std::int32_t, std::vector<Instruction>> _instructions;
private:
	// ���еĵݹ��ӳ���

	//token���
	std::optional<Token> nextToken();
	void unreadToken();
	std::vector<Token> _tokens;
	std::size_t _offset;

	//�ֲ�����������-ƫ��
	std::map<std::string, std::int32_t> _localVars;
	std::optional<std::int32_t> getIndexInLocal(const std::string&);
	std::int32_t _localIndex;
	void addVariable(const Token&);

	//ȫ�ָ�������������
	std::optional<std::int32_t> getIndexInGlobal(const std::string&);
	std::int32_t _globalIndex;
};