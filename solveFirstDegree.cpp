#include "computor.hpp"

void print(_Float128 fp) {
    char buf[32 + sizeof(".e+99999")];
    int sz = strfromf128(buf, sizeof buf, "%.32g", fp);
    fwrite(buf, 1, sz, stdout);
	std::cout << std::endl;
}

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

	int value = expressions[i].getValue();

	_Float128 res = 0;
	if (expressions.size() == 2)
		res = (_Float128)expressions[0].getValue() / value;
	std::cout << "The solution is:" << std::endl;
	print(res);
}