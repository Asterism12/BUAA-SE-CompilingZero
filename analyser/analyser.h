#pragma once
#include "tokenizer/token.h"
#include "instruction/instruction.h"

#include <any>
#include <cstdint>
#include <optional>
#include <vector>
#include <map>
#include <string>
class Analyser final {
public:
	Analyser(std::vector<Token> v)
		: _tokens(std::move(v)), _offset(0), _globalIndex(0),
		_currentFunction(0), _localIndex(0), _currentLine(0) {};

	// 唯一接口
	void Analyse();

	//程序结束后结果保存在这些数据结构中
	//常量表
	std::vector<std::any> _consts;
	//全局变量表，名字-偏移
	std::map<std::string, std::int32_t> _globalVars;
	//函数表，名字-函数序号
	std::map<std::string, std::int32_t> _functions;
	//函数对应的指令队列，函数序号-指令队列
	std::map<std::int32_t, std::vector<Instruction>> _instructions;
private:
	// 所有的递归子程序
	void analyse_C0_sprogram();
	void analyse_variable_declaration();
	void analyse_function_definition();
	char analyse_type_specifier();
	bool analyse_initializer(char type);
	void analyse_expression();
	void analyse_multiplicative_expression();
	bool analyse_additive_operator();
	void analyse_unary_expression();
	bool analyse_multiplicative_operator();

	//token相关
	std::optional<Token> nextToken();
	void unreadToken();
	std::vector<Token> _tokens;
	std::size_t _offset;

	//局部变量表，名字-偏移
	std::map<std::string, std::int32_t> _localVars;
	std::optional<std::int32_t> getIndexInLocal(const std::string&);
	std::int32_t _localIndex;
	std::int32_t _currentFunction;
	std::int32_t _currentLine;
	void addVariable(const Token&);
	void initializeVar(char type);

	//全局辅助变量及函数
	std::optional<std::int32_t> getIndexInGlobal(const std::string&);
	std::int32_t _globalIndex;
};