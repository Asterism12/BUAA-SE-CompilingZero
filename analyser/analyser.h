#pragma once
#include "tokenizer/token.h"
#include "instruction/instruction.h"

#include <any>
#include <cstdint>
#include <optional>
#include <vector>
#include <map>
class Analyser final {
public:
	Analyser(std::vector<Token> v)
		: _tokens(std::move(v)), _offset(0), _instructions({}) {}

	// 唯一接口
	void Analyse();

	//程序结束后结果保存在这些数据结构中
	//常量表
	std::vector<std::any> _consts;
	//全局变量表
	std::vector<std::string> _globalVars;
	//函数表，名字-函数序号
	std::map<std::string, std::int32_t> _functions;
	//函数对应的指令队列，函数序号-指令队列
	std::map<std::int32_t, std::vector<Instruction>> _instructions;
private:
	// 所有的递归子程序


	std::optional<Token> nextToken();
	void unreadToken();
	void addVariable(const Token&);
	int32_t getIndex(const std::string&);

	std::vector<Token> _tokens;
	std::size_t _offset;

	//局部变量表
	std::vector<std::string> _localVars;
};