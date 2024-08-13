#include "computor.hpp"

void	help(void)
{
	std::cout << std::endl << "Input format consists of multiple expressions within an equation." << std::endl;
	std::cout << "Each expression can be formulated like this: +/- M * X^E" << std::endl;
	std::cout << "M beeing the multiple, X being the equation's variable, and E the exponent." << std::endl << std::endl;
	std::cout << "Any expression's exponent needs to be between 0 and 2." << std::endl;
	std::cout << "An expression can be shortened/simplified, ex: + 1x^1 = 0 -> x = 0" << std::endl << std::endl;
	std::cout << "An equation needs to have 1 \"=\"." << std::endl;
	std::cout << "An equation can have any number of expressions" << std::endl;
	std::cout << "Spaces are not a requirement" << std::endl << std::endl;
	std::cout << "Equation example: \"24 *X^0 -1 * x^2 + 6x = 3x - 6\"" << std::endl;
	std::cout << "Equation after parsing: \"30 + 3X - 1X^2 = 0\"" << std::endl << std::endl;
}