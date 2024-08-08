#include "computor.hpp"

void	run(char **argv)
{
	std::string str = argv[1];
	try{
		Term test(str);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}