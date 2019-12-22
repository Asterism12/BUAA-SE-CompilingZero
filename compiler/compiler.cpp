#include "compiler.h"
//import from vm-cpp
void Compiler::writeNBytes(void* addr, int count) {
	char bytes[8];
	if (!(0 < count && count <= 8)) {
		throw Error("compiler error");
	}
	char* p = reinterpret_cast<char*>(addr) + (count - 1);
	for (int i = 0; i < count; ++i) {
		bytes[i] = *p--;
	}
	_wtr.write(bytes, count);
};

std::map<Operation, char*> ITCTable = {
	{Operation::ipush,"\x02"},//4
	{Operation::iadd,"\x30"},//0
	{Operation::isub,"\x34"},//0
	{Operation::imul,"\x38"},//0
	{Operation::idiv,"\x3c"},//0
	{Operation::loada,"\x0a"},//2,4
	{Operation::loadc,"\x09"},//2
	{Operation::call,"\x80"},//2
	{Operation::icmp,"\x44"},
	{Operation::je,"\x71"},
	{Operation::jne,"\x72"},
	{Operation::jg,"\x75"},
	{Operation::jge,"\x74"},
	{Operation::jl,"\x73"},
	{Operation::jle,"\x76"},
	{Operation::jmp,"\x70"},//2
	{Operation::nop,"\x00"},
	{Operation::pop,"\x04"},
	{Operation::ret,"\x88"},//0
	{Operation::iret,"\x89"},//0
	{Operation::iprint,"\xa0"},
	{Operation::iscan,"\xb0"},
	{Operation::iload,"\x10"},
	{Operation::istore,"\x20"},
	{Operation::ineg,"\x40"}
};

void Compiler::instructionToBinary(Instruction& ins) {
	if (auto it = ITCTable.find(ins.getOpr()); it != ITCTable.end()) {
		_wtr.write(it->second, 1);
	}
	else {
		throw Error("Unvalid operation type");
	}
	switch (ins.getOpr())
	{
	case Operation::ipush: {
		u4 param = ins.getParam().first;
		writeNBytes(&param, sizeof param);
		break;
	}
	case Operation::loada: {
		u2 param1 = ins.getParam().first;
		writeNBytes(&param1, sizeof param1);
		u2 param2 = ins.getParam().first;
		writeNBytes(&param2, sizeof param2);
		break;
	}
	case Operation::loadc: {
		u2 param = ins.getParam().first;
		writeNBytes(&param, sizeof param);
		break;
	}
	case Operation::call: {
		u2 param = ins.getParam().first;
		writeNBytes(&param, sizeof param);
		break;
	}
	case Operation::jmp: {
		u2 param = ins.getParam().first;
		writeNBytes(&param, sizeof param);
		break;
	}
	default:
		break;
	}
}

void Compiler::Compile() {
	writeAll();
	std::cout << "Compiler successful return." << std::endl;
}

void Compiler::writeAll() {
	//magic number
	_wtr.write("\x43\x30\x3A\x29", 4);
	//version
	_wtr.write("\x00\x00\x00\x01", 4);
	//const
	u2 constNum = _consts.size();
	std::string test;
	writeNBytes(&constNum, sizeof constNum);
	for (auto constant : _consts) {
		if (constant.type() == typeid(int)) {
			_wtr.write("\x01", 1);
			int_t v = std::any_cast<int>(constant);
			writeNBytes(&v, sizeof v);
		}
		else if (constant.type() == typeid(std::string)) {
			_wtr.write("\x00", 1);
			std::string v = std::any_cast<std::string>(constant);
			u2 len = v.length();
			writeNBytes(&len, sizeof len);
			_wtr.write(v.c_str(), len);
		}
		else {
			throw Error("Unvalid typeid");
		}
	}
	//start code
	u2 instructions_count = _startInstructions.size();
	writeNBytes(&instructions_count, sizeof instructions_count);
	for (Instruction ins : _startInstructions) {
		instructionToBinary(ins);
	}
	//function table
	u2 functions_count = _functionNameConstant.size();
	writeNBytes(&functions_count, sizeof functions_count);
	//function instructions table
	for (int i = 0; i < _instructions.size(); i++) {
		u2 functions_name_index = _functionNameConstant[i];
		writeNBytes(&functions_name_index, sizeof functions_name_index);
		std::string function_name = std::any_cast<std::string>(_consts[_functionNameConstant[i]]);
		u2 functions_param_size = _functionParameter[function_name].size();
		writeNBytes(&functions_param_size, sizeof functions_param_size);
		u2 functions_level = 1;
		writeNBytes(&functions_level, sizeof functions_level);
		u2 functions_ins_count = _instructions[i].size();
		writeNBytes(&functions_ins_count, sizeof functions_ins_count);
		for (auto ins : _instructions[i]) {
			instructionToBinary(ins);
		}
	}
}