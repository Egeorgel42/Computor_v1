#include "computor.hpp"

int main(int argc, char **argv)
{
	if (argc == 1)
		help();
	else if (argc == 2)
		run(argv);
	else
		std::cout << "Invalid number of arguments" << std::endl;
}