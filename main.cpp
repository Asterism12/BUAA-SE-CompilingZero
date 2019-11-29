#include <iostream>
#include "help/help.h"

int main(int argc, char** argv) {

	switch (argc)
	{
	case 2:
		if (argv[1] == "-h") {
			getHelp();
		}
		else {
			//TODO
		}
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		std::cout << "Error : Wrong parameter list"<<std::endl;
		getHelp();
		break;
	}
}