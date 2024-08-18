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

void	add(std::vector<Term> &expressions, bool *bx)
{
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
				it = expressions.erase(x[exponent]);
				it = expressions.erase(it);
			}
		}
	}
	if (!bx[0] && !bx[1] && !bx[2])
		throw std::runtime_error("Equation cannot result in \"0 = 0\"");
	if (!bx[1] && !bx[2])
		throw std::runtime_error("At least 1 X is expected");
	std::sort(expressions.begin(), expressions.end(), [] (Term &a, Term &b) {return a.getExponent() < b.getExponent();});
}

void	print(std::vector<Term> &expressions, bool *bx)
{
	int i = -1;
	if (bx[0] && expressions[++i].getValue())
		std::cout << " ( " << expressions[i].getValue() << " ) ";
	if (bx[1] && expressions[++i].getValue())
		std::cout << " ( " << expressions[i].getValue() << "X" << " ) ";
	if (bx[2] && expressions[++i].getValue())
		std::cout << " ( " << expressions[i].getValue() << "X^2" << " ) ";
	std::cout << " = 0" << std::endl;
}

void	run(std::string str)
{
	std::vector<Term> expressions;

	bool bx[3] = {false, false, false};

	parse(str, expressions);
	add(expressions, bx);
	print(expressions, bx);
}