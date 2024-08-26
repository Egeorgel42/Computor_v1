#include "computor.hpp"

void printSecondDegree(bool isDNull)
{
	if (isDNull)
		std::cout << "Discriminant is strictly equal to 0, the solution is:" << std::endl;
	else
		std::cout << "Discriminant is strictly positive, the two solutions are:" << std::endl;
}

void calculateSecondDegree(std::vector<Term> &expressions)
{
	int bx[3] = {-1, -1, -1};
	int j = 0;
	mpf_class bsquared = 0;
	mpf_class D = 0;
	mpf_class pos_dividend = 0;
	mpf_class neg_dividend = 0;
	mpz_class a, b, c;
	c = 0;
	b = 0;
	a = 0;

	for (int i = 0; i < 3; i++)
	{
		if (i < (int)expressions.size() && expressions[j].getExponent() == i)
		{
			bx[i] = j;
			j++;
		}
	}
	
	for (int i = 0; i < (int)expressions.size(); i++)
	{
		if (i == 0 && bx[i] != -1)
			c = expressions[bx[i]].getValue();
		else if (i == 1 && bx[i] != -1)
		{
			b = expressions[bx[i]].getValue() * -1;
			bsquared = b * b;
		}
		else if (i == 2 && bx[i] != -1)
			a = expressions[bx[i]].getValue();
	}

	if ((bsquared -= 4 * a * c) < 0)
	{
		std::cout << "Discriminant is strictly negative, this is unsolvable.";
		return;
	}

	mpf_sqrt(D.get_mpf_t(), bsquared.get_mpf_t());
	pos_dividend = b + D;
	neg_dividend = b - D;

	if (pos_dividend == neg_dividend)
	{
		printSecondDegree(true);
		std::cout << pos_dividend / (a * 2) << std::endl;
	}
	else
	{
		printSecondDegree(false);
		std::cout << pos_dividend / (a * 2) << std::endl;
		std::cout << neg_dividend / (a * 2) << std::endl;
	}
}