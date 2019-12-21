#pragma once
#include <cstdint>
#include <utility>
enum class Operation {
	ipush,
	iadd,
	isub,
	imul,
	idiv,
	loada,
	loadc,
	call,
	icmp,
	je,
	jne,
	jg,
	jge,
	jl,
	jle,
	jmp,
	nop,
	pop,
	ret,
	iret,
	iprint,
	iscan,
	iload,
	istore,
	ineg
};

class Instruction final {
public:
	Instruction(Operation opr, std::int32_t x, std::int32_t y) : _opr(opr), _x(x), _y(y), _paramNum(2) {}
	Instruction(Operation opr, int32_t x) : _opr(opr), _x(x), _y(0), _paramNum(1) {}
	Instruction(Operation opr) : _opr(opr), _x(0), _y(0), _paramNum(0) {}
	Instruction& operator=(Instruction i) { swap(*this, i); return *this; }
	Operation getOpr();
	std::int32_t getParamNum();
	std::pair<std::int32_t, std::int32_t> getParam();
private:
	Operation _opr;
	std::int32_t _x;
	std::int32_t _y;
	std::int32_t _paramNum;

	inline void swap(Instruction& lhs, Instruction& rhs) {
		using std::swap;
		swap(lhs._opr, rhs._opr);
		swap(lhs._x, rhs._x);
		swap(lhs._y, rhs._y);
	}
};