#pragma once
#include "instruction/instruction.h"
#include "error/error.h"

#include <vector>
#include <map>
#include <string>
#include <any>
#include <fstream>
#include <iostream>
class Assembler {
public:
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
};