#include "computor.hpp"

void	parse(std::string str, std::vector<Term> &expressions)
{
	bool ispositive = true;
	std::regex split = std::regex(R"((\=|[\-\+]?[^\=\-\+]*))");
	std::regex_token_iterator<std::string::iterator> it(str.begin(), str.end(), split);
	std::regex_token_iterator<std::string::iterator> end;
	std::regex_token_iterator<std::string::iterator> buff;

	if (it == end)
		throw std::runtime_error("Empty value");
	if (*it == "=")
		throw std::runtime_error("Empty value before \"=\" Sign");
	for (; it != end; it++)
	{
		std::string strbuf = *it;
		if (strbuf.size() == 0)
			continue;
		if (strbuf == "=" && !ispositive)
			throw std::runtime_error("Unexpected second \"=\" Sign");
		else if (strbuf == "=")
		{
			ispositive = false;
			buff = it;
			continue;
		}
		try{
			Term expression(strbuf, ispositive);
			expressions.push_back(expression);
		}
		catch(Term::EmptyValue &e) {}
	}
	if (buff == it)
		throw std::runtime_error("Empty value after \"=\" Sign");
	else if (ispositive)
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
			*(x[exponent]) += *it;
			it = expressions.erase(it);
		}
	}
	if (!bx[0] && !bx[1] && !bx[2])
		throw std::runtime_error("No expressions given");
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