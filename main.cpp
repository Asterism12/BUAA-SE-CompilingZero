#include "argparse.hpp"

#include "error/error.h"
#include "tokenizer/tokenizer.h"
#include "analyser/analyser.h"
#include "assembler/assembler.h"
#include "compiler/compiler.h"

#include <fstream>
#include <iostream>
#include <string.h>

void tokenize(std::istream& input, std::ostream& output) {
	Tokenizer tkz(input);
	try {
		std::vector<Token> tokens = tkz.Tokenize();
		for (Token t : tokens) {
			if (t.GetType() == TokenType::RESERVED_WORD) {
				std::cout << std::any_cast<std::string>(t.GetValue()) << "\treserved word\t" << t.GetLine() << std::endl;
			}
			else if (t.GetType() == TokenType::IDENTIFIER) {
				std::cout << std::any_cast<std::string>(t.GetValue()) << "\tidentifier\t" << t.GetLine() << std::endl;
			}
			else if (t.GetType() == TokenType::UNSIGNED_INTEGER) {
				std::cout << std::any_cast<int>(t.GetValue()) << "\tinteger\t\t" << t.GetLine() << std::endl;
			}
			else {
				try {
					std::cout << std::any_cast<char>(t.GetValue()) << "\tsign\t\t" << t.GetLine() << std::endl;
				}
				catch (const std::bad_any_cast&) {
					std::cout << std::any_cast<std::string>(t.GetValue()) << "\tsign\t\t" << t.GetLine() << std::endl;
				}
			}
		}
	}
	catch (Error err) {
		err.printErrorMessage();
	}
}

void analyse(std::istream& input, std::ostream& output) {
	Tokenizer tkz(input);
	Analyser as(tkz.Tokenize());
	as.Analyse();
	try {
		//as.Analyse();
		std::cout << "const table" << std::endl;
		for (std::any c : as._consts) {
			try {
				std::cout << std::any_cast<char>(c) << "\tchar" << std::endl;
				continue;
			}
			catch(const std::bad_any_cast&){}
			try {
				std::cout << std::any_cast<int>(c) << "\tint" << std::endl;
				continue;
			}
			catch (const std::bad_any_cast&) {}
			try {
				std::cout << std::any_cast<std::string>(c) << "\tstring" << std::endl;
				continue;
			}
			catch (const std::bad_any_cast&) {
				throw Error("type error");
			}
		}
	}
	catch (Error err) {
		err.printErrorMessage();
	}
}

void compiler(std::istream& input, std::ostream& output) {
	try {
		Tokenizer tkz(input);
		Analyser as(tkz.Tokenize());
		as.Analyse();
		Compiler cp(output, as);
		cp.Compile();
	}
	catch (Error err) {
		err.printErrorMessage();
	}
}

void assembler(std::istream& input, std::ostream& output) {
	try {
		Tokenizer tkz(input);
		Analyser as(tkz.Tokenize());
		as.Analyse();
		Assembler ab(output, as);
		ab.Assemble();
	}
	catch (Error err) {
		err.printErrorMessage();
	}
}

int main(int argc, char** argv) {

	argparse::ArgumentParser program("cc0");

	program.add_argument("-t")
		.help("tokenizer test")
		.default_value(false)
		.implicit_value(true);

	program.add_argument("-a")
		.help("analyser test")
		.default_value(false)
		.implicit_value(true);

	program.add_argument("-s")
		.help("Translate the input c0 source code into a text assembly file")
		.default_value(false)
		.implicit_value(true);

	program.add_argument("-c")
		.help("Translate the input c0 source code into a binary object file")
		.default_value(false)
		.implicit_value(true);

	program.add_argument("-h")
		.help("Shows help with compiler usage")
		.default_value(false)
		.implicit_value(true);

	program.add_argument("-o")
		.default_value(std::string("out"))
		.required()
		.help("specify the output file.");

	program.add_argument("input")
		.help("specify the input file.");


	try {
		program.parse_args(argc, argv);
	}
	catch (const std::runtime_error & err) {
		std::cout << err.what() << std::endl;
		std::cout << program;
		exit(0);
	}

	auto input_file = program.get<std::string>("input");
	auto output_file = program.get<std::string>("-o");
	std::istream* input;
	std::ostream* output;
	std::ifstream inf;
	std::ofstream outf;
	if (input_file != "-") {
		inf.open(input_file, std::ios::in);
		if (!inf) {
			std::cout << "Fail to open "+ input_file+" for reading.\n";
			exit(2);
		}
		input = &inf;
	}
	else
		input = &std::cin;
	if (output_file != "-") {
		if (program["-c"] == true) {
			outf.open(output_file, std::ios::binary| std::ios::trunc);
		}
		else {
			outf.open(output_file, std::ios::out | std::ios::trunc);
		}
		if (!outf) {
			std::cout << "Fail to open " + output_file + " for writing.\n";
			exit(2);
		}
		output = &outf;
	}
	else
		output = &std::cout;

	//for test
	if (program["-t"] == true) {
		tokenize(*input, *output);
	}
	if (program["-a"] == true) {
		analyse(*input, *output);
	}

	if (program["-s"] == true && program["-c"] == true) {
		std::cout << "You can only perform tokenization or syntactic analysis at one time.";
		exit(2);
	}
	if (program["-s"] == true) {
		assembler(*input, *output);
	}
	else if (program["-c"] == true) {
		compiler(*input, *output);
	}
	else {
		std::cout << "You must choose -c or -s.";
		exit(2);
	}

	return 0;
}