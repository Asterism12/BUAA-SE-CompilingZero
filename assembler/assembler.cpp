#include "assembler.h"

std::map<Operation, std::string> ITATable = {
	{Operation::ipush,"ipush"},
	{Operation::iadd,"iadd"},
	{Operation::isub,"isub"},
	{Operation::imul,"imul"},
	{Operation::idiv,"idiv"},
	{Operation::loada,"loada"},
	{Operation::loadc,"loadc"},
	{Operation::call,"call"},
	{Operation::icmp,"icmp"},
	{Operation::je,"je"},
	{Operation::jne,"jne"},
	{Operation::jg,"jg"},
	{Operation::jge,"jge"},
	{Operation::jl,"jl"},
	{Operation::jle,"jle"},
	{Operation::jmp,"jmp"},
	{Operation::nop,"nop"},
	{Operation::pop,"pop"},
	{Operation::ret,"ret"},
	{Operation::iret,"iret"},
	{Operation::iprint,"iprint"},
	{Operation::iscan,"iscan"},
	{Operation::iload,"iload"},
	{Operation::istore,"istore"},
	{Operation::ineg,"ineg"}
};

std::string Assembler::instructionToAssembly(Instruction & ins) {
	std::string ret;
	if (auto it = ITATable.find(ins.getOpr()); it != ITATable.end()) {
		ret += it->second + '\t';
	}
	else {
		throw Error("Unvalid operation type");
	}
	switch (ins.getParamNum())
	{
	case 0:
		break;
	case 1:
		ret += std::to_string(ins.getParam().first);
		break;
	case 2:
		ret += std::to_string(ins.getParam().first) + ", " + std::to_string(ins.getParam().second);
		break;
	default:
		break;
	}
	return ret;
}

void Assembler::Assemble() {
	writeAll();
	std::cout << "Assembler successful return." << std::endl;
}

void Assembler::writeAll() {
	//const
	_wtr << ".constants:" << '\n';
	for (int i = 0; i < _consts.size(); i++) {
		if (_consts[i].type() == typeid(int)) {
			_wtr << i << "\tI\t" << std::any_cast<int>(_consts[i]) << '\n';
		}
		else if (_consts[i].type() == typeid(std::string)) {
			_wtr << i << "\tS\t" << std::any_cast<std::string>(_consts[i]) << '\n';
		}
		else {
			throw Error("Unvalid typeid");
		}
	}
	//start code
	_wtr << ".start:" << '\n';
	for (int i = 0; i < _startInstructions.size(); i++) {
		_wtr << i << '\t' << instructionToAssembly(_startInstructions[i]) << '\n';
	}
	//function table
	_wtr << ".functions:" << '\n';
	for (int i = 0; i < _instructions.size(); i++) {
		std::string function_name = std::any_cast<std::string>(_consts[_functionNameConstant[i]]);
		_wtr << i << '\t' << _functionNameConstant[i] << '\t' << _functionParameter[function_name].size() << "\t1\n";
	}
	//function instructions table
	for (int i = 0; i < _instructions.size(); i++) {
		_wtr << ".F" << i << ":\t#" << std::any_cast<std::string>(_consts[_functionNameConstant[i]]) << '\n';
		for (int j = 0; j < _instructions[i].size(); j++) {
			_wtr << j << '\t' << instructionToAssembly(_instructions[i][j]) << '\n';
		}
	}
}