#include "computor.hpp"

static void	revX(std::vector<Term> &expressions, int i)
{
	if (expressions[i].getValue() > 0)
		return;
	for (std::vector<Term>::iterator it = expressions.begin(); it != expressions.end(); it++)
		it->reverseValue();
}

void	calculateFirstDegree(std::vector<Term> &expressions)
{
	int i = expressions.size() - 1;
	revX(expressions, i);

	mpf_class value = expressions[i].getValue();

	if (expressions.size() == 2)
		value /= expressions[0].getValue();
	std::cout << "The solution is:" << std::endl;
	std::cout << value << std::endl;
}