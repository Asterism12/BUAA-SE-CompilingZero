#include "help.h"
void getHelp() {
	std::string str = R"(
Usage:
  cc0 [options] input [-o file]
or 
  cc0 [-h]
Options:
  -s        Translate the input c0 source code into a text assembly file
  -c        Translate the input c0 source code into a binary object file
  -h        Shows help with compiler usage
  -o file   Output to the specified file

When no parameters are provided, the default is -h
If input does not provide -o file, the default is -o out
)";
	std::cout << str << std::endl;
}