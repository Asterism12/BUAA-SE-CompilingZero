#pragma once
#include <sstream>
enum class Operation {
	ILL,
	LIT,
	LOD,
	STO,
	ADD,
	SUB,
	MUL,
	DIV,
	WRT
};

class Instruction final {
public:
	Instruction(Operation opr, std::int32_t x, std::int32_t y) : _opr(opr), _x(x), _y(y) {}
	Instruction(Operation opr, int32_t x) : _opr(opr), _x(x), _y(0) {}
private:
	Operation _opr;
	std::int32_t _x;
	std::int32_t _y;
};