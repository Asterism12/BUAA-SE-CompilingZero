#include "instruction.h"

Operation Instruction::getOpr() {
	return _opr;
}
std::int32_t Instruction::getParamNum() {
	return _paramNum;
}
std::pair<std::int32_t, std::int32_t> Instruction::getParam() {
	return { _x,_y };
}