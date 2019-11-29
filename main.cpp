#include <iostream>
#include <string.h>
#include <functional>
#include "help/help.h"
#include "error/error.h"

int main(int argc, char** argv) {
	switch (argc)
	{
	case 2:
		if (!strcmp(argv[1], "-h")) {
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
		Error e = Error(std::string("Wrong parameter list."),ErrorType::ParameterError);
		e.printErrorMessage();
		getHelp();
		break;
	}

	return 0;
}