#include "computor.hpp"

int main(int argc, char **argv)
{
	if (argc == 1)
		help();
	else if (argc == 2)
	{
		try{
			run(argv[1]);
		}
		catch (std::exception &e){
			std::cout << e.what() << std::endl;
		}
	}
	else
		std::cout << "Invalid number of arguments" << std::endl;
}