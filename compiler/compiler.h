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
class Compiler {
public:
	Compiler(std::ostream& wtr, Analyser as) :_wtr(wtr) {
		_consts = as._consts;
		_startInstructions = as._startInstructions;
		_instructions = as._instructions;
		_functionNameConstant = as._functionNameConstant;
		_functionParameter = as._functionParameter;
	}
	void Compile();
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
	std::string instructionToBinary(Instruction&);
};