#include <iostream>
#include <string.h>
#include "help/help.h"

int main(int argc, char** argv) {

	switch (argc)
	{
	case 2:
		if (!strcmp(argv[1],"-h")) {
			getHelp();
		}
		else {
			//TODO
			std::cout << argv[1] << std::endl;
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
	return 0;
}