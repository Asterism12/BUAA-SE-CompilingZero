#include "help.h"
void getHelp() {
	std::string str = R"(
Usage:
  cc0 [options] input [-o file]
or 
  cc0 [-h]
Options:
  -s        将输入的 c0 源代码翻译为文本汇编文件
  -c        将输入的 c0 源代码翻译为二进制目标文件
  -h        显示关于编译器使用的帮助
  -o file   输出到指定的文件 file

不提供任何参数时，默认为 -h
提供 input 不提供 -o file 时，默认为 -o out)";
	std::cout << str << std::endl;
}