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
		_currentFunction(-1), _localIndex(0), _currentLine(0) {};

	// 唯一接口
	void Analyse();

	//程序结束后结果保存在这些数据结构中
	//常量表
	std::vector<std::any> _consts;
	//start_code
	std::vector<Instruction> _startInstructions;
	//函数对应的指令队列，函数序号-指令队列
	std::vector<std::vector<Instruction>> _instructions;
private:
	// 所有的递归子程序
	void analyse_C0_sprogram();
	void analyse_variable_declaration();
	void analyse_function_definition();
	char analyse_type_specifier();
	bool analyse_initializer(char type);
	void analyse_expression();
	void analyse_multiplicative_expression();
	void analyse_unary_expression();
	void analyse_function_call();
	void analyse_parameter_clause();
	void analyse_compound_statement();

	//token相关
	std::optional<Token> nextToken();
	void unreadToken();
	std::vector<Token> _tokens;
	std::size_t _offset;
	std::int32_t _currentLine;

	//局部变量表，名字-<是否是常量,偏移>
	std::map<std::string, std::pair<bool, std::int32_t>> _localVars;
	std::optional<std::int32_t> getIndexInLocal(const std::string&);
	std::int32_t _localIndex;
	

	//全局变量表，名字-<是否是常量,偏移>
	std::map<std::string, std::pair<bool, std::int32_t>> _globalVars;
	std::optional<std::int32_t> getIndexInGlobal(const std::string&);
	std::int32_t _globalIndex;

	//变量相关
	std::int32_t addConstant(const Token&);
	void addInstruction(Instruction instruction);
	void addVariable(const std::string&, bool);
	bool loadVariable(const std::string&);
	void initializeVariable(char type);
	
	//函数相关
	std::map<std::string, std::int32_t> _functions;
	std::map<std::string, std::vector<char>> _functionParameter;
	std::int32_t _currentFunction;
	void addFunction(const std::string&);
	int getFunctionIndex(const std::string&);
	std::vector<char> getFunctionParameter(const std::string&);
};