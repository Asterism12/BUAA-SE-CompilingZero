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
	//输出流
	std::ostream& _wtr;
	//常量表
	std::vector<std::any> _consts;
	//start_code
	std::vector<Instruction> _startInstructions;
	//函数对应的指令队列，函数序号-指令队列
	std::vector<std::vector<Instruction>> _instructions;

	void writeAll();
};