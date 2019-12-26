# BUAA-SE-CompilingZero
手动实现一个完整的 C0 编译器  并且能够输出在标准虚拟机或其他目标机上可运行的代码。

编译器所使用的虚拟机、虚拟机标准、以及所依赖的文法和界面标准见https://github.com/BUAA-SE-Compiling

---

## How to build
git clone git@github.com:Asterism12/BUAA-SE-CompilingZero.git
cd BUAA-SE-CompilingZero
git submodule update --init --recursive
mkdir build
cd build
cmake ..
make

## How to use
Usage: cc0 [options] input

Positional arguments:
input specify the input file.

Optional arguments:
-h --help show this help message and exit
-s Translate the input c0 source code into a text assembly file
-c Translate the input c0 source code into a binary object file
-h Shows help with compiler usage
-o specify the output file.[Required]

## 3rd party
argparse https://github.com/p-ranav/argparse
