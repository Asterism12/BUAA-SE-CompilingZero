#include "assembler.h"

std::string Assembler::instructionToAssembly(Instruction& ins) {
	return "ins";
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
			_wtr << i << "\tINT\t" << std::any_cast<int>(_consts[i]) << '\n';
		}
		else if (_consts[i].type() == typeid(char)) {
			_wtr << i << "\tCHAR\t" << std::any_cast<char>(_consts[i]) << '\n';
		}
		else if (_consts[i].type() == typeid(std::string)) {
			_wtr << i << "\tSTRING\t" << std::any_cast<std::string>(_consts[i]) << '\n';
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
		_wtr << i << '\t' << _functionNameConstant[i] << '\t' << _functionParameter.size() << "\t1\n";
	}
	//function instructions table
	for (int i = 0; i < _instructions.size(); i++) {
		_wtr << ".F" << i << ":\t#" << std::any_cast<std::string>(_consts[_functionNameConstant[i]]) << '\n';
		for (int j = 0; j < _instructions[i].size(); j++) {
			_wtr << j << '\t' << instructionToAssembly(_instructions[i][j]) << '\n';
		}
	}
}