#pragma once
enum class Operation {
	ILL = 0,
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
	Instruction(Operation opr, std::int32_t x) : _opr(opr), _x(x) {}

	Instruction() : Instruction(Operation::ILL, 0) {}
	Instruction(const Instruction& i) { _opr = i._opr; _x = i._x; }
	Instruction(Instruction&& i) :Instruction() { swap(*this, i); }
	Instruction& operator=(Instruction i) { swap(*this, i); return *this; }
	bool operator==(const Instruction& i) const { return _opr == i._opr && _x == i._x; }

	Operation GetOperation() const { return _opr; }
	std::int32_t GetX() const { return _x; }
private:
	Operation _opr;
	std::int32_t _x;
};