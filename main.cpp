#include<iostream>

int main(int argc, char** argv) {

	for (int i = 0; i < argc; i++)
		std::cout << "Argument " << i << " is " << argv[i] << std::endl;
	return EXIT_SUCCESS;
}