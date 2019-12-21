#include "assembler.h"

void Assembler::Assemble() {
	writeAll();
}

void Assembler::writeAll() {
	//const
	_wtr << ".constants:" << '\n';
	for (int i = 0; i < _consts.size(); i++) {
		if (_consts[i].type()==typeid(int)) {
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
		//dosomething
	}
	//function table
	_wtr << ".functions:" << '\n';
	for (int i = 0; i < _instructions.size(); i++) {
		_wtr << i << '\t' << _functionNameConstant[i] << '\t' << _functionParameter.size() << "\t1\n";
	}
	//function instructions table
	for (int i = 0; i < _instructions.size(); i++) {
		_wtr << 'F' << i << ":\t#" << std::any_cast<std::string>(_consts[i]) << '\n';
		for (int j = 0; j < _instructions[i].size(); i++) {
			//_wtr<<j<<
		}
	}
}