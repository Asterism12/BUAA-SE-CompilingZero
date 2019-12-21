#pragma once
#include "instruction/instruction.h"
#include "analyser/analyser.h"
#include "error/error.h"

#include <vector>
#include <map>
#include <string>
#include <any>
#include <fstream>
#include <iostream>
class Assembler {
private:
	using u1 = std::uint8_t;
	using u2 = std::uint16_t;
	using u4 = std::uint32_t;
	using u8 = std::uint64_t;

	#define U1_MAX UINT8_MAX
	#define U2_MAX UINT16_MAX
	#define U4_MAX UINT32_MAX
	#define U8_MAX UINT64_MAX

	using i1 = std::int8_t;
	using i2 = std::int16_t;
	using i4 = std::int32_t;
	using i8 = std::int64_t;

public:
	Assembler(std::ostream& wtr, Analyser as) :_wtr(wtr) {
		_consts = as._consts;
		_startInstructions = as._startInstructions;
		_instructions = as._instructions;
		_functionNameConstant = as._functionNameConstant;
		_functionParameter = as._functionParameter;
	}
	void Assemble();
private:
	//�����
	std::ostream& _wtr;
	//������
	std::vector<std::any> _consts;
	//start_code
	std::vector<Instruction> _startInstructions;
	//������Ӧ��ָ����У��������-ָ�����
	std::vector<std::vector<Instruction>> _instructions;
	//����-������Ӧ��
	std::vector<int> _functionNameConstant;
	//����������
	std::map<std::string, std::vector<char>> _functionParameter;

	void writeAll();
	std::string instructionToAssembly(Instruction&);
};