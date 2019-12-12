#include "argparse.hpp"
#include <fstream>
#include <iostream>
#include <string.h>
#include "error/error.h"

int main(int argc, char** argv) {

	argparse::ArgumentParser program("program name");

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
		outf.open(output_file, std::ios::out | std::ios::trunc);
		if (!outf) {
			std::cout << "Fail to open " + output_file + " for writing.\n";
			exit(2);
		}
		output = &outf;
	}
	else
		output = &std::cout;

	if (program["-s"] == true && program["-c"] == true) {
		std::cout << "You can only perform tokenization or syntactic analysis at one time.";
		exit(2);
	}
	if (program["-s"] == true) {
		//
	}
	else if (program["-c"] == true) {
		//
	}
	else {
		std::cout << "You must choose -c or -s.";
		exit(2);
	}

	return 0;
}