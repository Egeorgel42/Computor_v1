#include "computor.hpp"

void	parse(std::string str, std::vector<Term> &expressions)
{
	bool ispositive = true;
	std::regex split = std::regex(R"((\=|[\-\+]?[^\=\-\+]*))");
	std::regex_token_iterator<std::string::iterator> it(str.begin(), str.end(), split);
	std::regex_token_iterator<std::string::iterator> end;

	if (it == end)
		throw std::runtime_error("Empty value");
	if (*it == "=")
		throw std::runtime_error("Empty value before \"=\" Sign");
	for (;it != end; it++)
	{
		std::string strbuf = *it;
		if (strbuf == "=" && !ispositive)
			throw std::runtime_error("Unexpected second \"=\" Sign");
		else if (strbuf == "=")
		{
			std::regex_token_iterator<std::string::iterator> next;
			ispositive = false;
			next = it;
			std::string nextstr = *(++next);
			if(nextstr.size() == 0)
				throw std::runtime_error("Empty value after \"=\" Sign");
			continue;
		}
		try{
			Term expression(strbuf, ispositive);
			expressions.push_back(expression);
		}
		catch(Term::EmptyValue &e) {}
	}
	if (ispositive)
		throw std::runtime_error("No \"=\" Sign found");
}

void	add(std::vector<Term> &expressions)
{
	bool bx[3] = {false, false, false};
	std::vector<Term>::iterator x[3];

	for (std::vector<Term>::iterator it = expressions.begin(); it != expressions.end();)
	{
		int exponent = it->getExponent();
		if (!bx[exponent])
		{
			x[exponent] = it;
			bx[exponent] = true;
			it++;
		}
		else
		{
			try{
				*(x[exponent]) += *it;
				it = expressions.erase(it);
			}
			catch (Term::EmptyValue &e)
			{
				bx[exponent] = false;
				it = expressions.erase(it);
			}
		}
	}
	for (std::vector<Term>::iterator it = expressions.begin(); it != expressions.end();)
	{
		if (!bx[it->getExponent()])
			it = expressions.erase(it);
		else
			it++;
	}
	if (!bx[0] && !bx[1] && !bx[2])
		throw std::runtime_error("Equation cannot result in \"0 = 0\"");
	if (!bx[1] && !bx[2])
		throw std::runtime_error("At least 1 X is expected after simplifation of equation");
	std::sort(expressions.begin(), expressions.end(), [] (Term &a, Term &b) {return a.getExponent() < b.getExponent();});
}

void	print(std::vector<Term> &expressions)
{
	bool first = true;
	for (long unsigned int i = 0; i < expressions.size(); i++)
	{
		if (expressions[i].getValue() > 0 && !first)
			std::cout << "+ ";
		else if (expressions[i].getValue() < 0 && first)
			std::cout << "-";
		else if (expressions[i].getValue() < 0)
			std::cout << "- ";
		std::cout << expressions[i];
		first = false;
	}
	std::cout << "= 0" << std::endl;
}

void	revSigns(std::vector<Term> &expressions)
{
	int i = 0;
	for (std::vector<Term>::iterator it = expressions.begin(); it != expressions.end(); it++)
	{
		if (it->getValue() > 0)
			i++;
		else
			i--;
	}
	if (i >= 0)
		return;
	for (std::vector<Term>::iterator it = expressions.begin(); it != expressions.end(); it++)
		it->reverseValue();
}

void	run(std::string str)
{
	std::vector<Term> expressions;

	parse(str, expressions);
	add(expressions);
	revSigns(expressions);
	print(expressions);
	if (std::find_if(expressions.begin(), expressions.end(), [](Term e){return e.getExponent() == 1;}) != expressions.end() &&
		std::find_if(expressions.begin(), expressions.end(), [](Term e){return e.getExponent() == 2;}) == expressions.end())
	{
		std::cout << "Polynomial degree: 1" << std::endl;
		calculateFirstDegree(expressions);
	}
	else if (std::find_if(expressions.begin(), expressions.end(), [](Term e){return e.getExponent() == 2;}) != expressions.end())
	{
		std::cout << "Polynomial degree: 2" << std::endl;
		calculateSecondDegree(expressions);
	}
}