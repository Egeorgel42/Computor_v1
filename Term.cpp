#include "computor.hpp"

static void parseSign(std::string str, uint &step, uint &step_mem, bool &ispositive)
{
	if (step_mem == Sign)
	{
		if (str == "+")
			ispositive = true;
		else if (str == "-")
			ispositive = false;
		else
			step_mem = Multiplicator;
		step = Multiplicator;
	}
	else if (str == "-" || str == "+")
		throw std::runtime_error("Unexpected Sign/Operator: " + str);
	if (step_mem == Multiplication)
	{
		if (str != "*")
			throw std::runtime_error("Expected Sign/Operator: *");
		step = X;
	}
	else if (str == "*")
		throw std::runtime_error("Unexpected Sign/Operator: *");
}

static void parseX(std::string str, uint &step, uint &step_mem, int &exponent)
{
	std::regex x1 = std::regex(R"([xX]\^\d+)");
	std::regex x2 = std::regex(R"([xX])");
	if (step_mem == X && std::regex_match(str, x1))
	{
		str.erase(0,2);
		try {
			exponent = std::stol(str);
		}
		catch(std::out_of_range &e){
			throw std::runtime_error("Long Overflow");
		}
		step = End;
	}
	else if (step_mem == X && std::regex_match(str, x2))
	{
		exponent = 1;
		step = End;
	}
	else if (step_mem != X && (std::regex_match(str,x1) || std::regex_match(str,x2)))
		throw std::runtime_error("Unexpected X");
	if (exponent > 2)
		throw std::runtime_error("Exponent of X needs to be between 0 and 2");
}

static void	parseMultiplicator(std::string str, uint &step, uint &step_mem, long &multiple, int &exponent)
{
	std::regex multiplicator = std::regex(R"(\d+)");
	std::regex specialCase = std::regex(R"((\d+[xX]|\d+[xX]\^\d+))");
	std::regex x = std::regex(R"(([xX]|[xX]\^\d+))");
	if (step_mem == Multiplicator && std::regex_match(str, x))
	{
		step = X;
		step_mem = X;
		multiple = 1;
	}
	else if (step_mem == Multiplicator && std::regex_match(str, specialCase))
	{
		size_t pos = 0;
		try {
			multiple = std::stol(str, &pos);
		}
		catch(std::out_of_range &e){
			throw std::runtime_error("Long Overflow");
		}
		step = X;
		step_mem = X;
		parseX(str.substr(pos, SIZE_MAX), step, step_mem, exponent);
		step = End;
	}
	else if (step_mem == Multiplicator && std::regex_match(str, multiplicator))
	{
		try {
			multiple = std::stol(str);
		}
		catch (std::out_of_range &e) {
			throw std::runtime_error("Long Overflow");
		}
		step = Multiplication;
	}
	else if (step_mem == Multiplicator)
		throw std::runtime_error("Unexpected Expression");
}

void	Term::ParseAndAdd(std::string val)
{
	bool ispositive = true;
	int exponent = -1;
	long multiple = -1;
	std::regex split = std::regex(R"(\s+)");
	std::smatch match;
	std::regex unexpectedCharacter = std::regex(R"(([xX\-\+\^\*]|[0-9])*)");
	std::regex_token_iterator<std::string::iterator> it(val.begin(), val.end(), split, -1);
	std::regex_token_iterator<std::string::iterator> end;
	if (it == end)
		throw std::runtime_error("Empty value");
	uint step = Sign;
	uint step_mem;
	for (; it != end; it++)
	{
		std::cout << "\"" << *it << "\"" << std::endl;
		step_mem = step;
		if (step == End)
			throw std::runtime_error("Unexpected Expression");
		std::string str = *it;
		parseSign(str, step, step_mem, ispositive);
		parseMultiplicator(str, step, step_mem, multiple, exponent);
		if (step != End)
			parseX(str, step, step_mem, exponent);
		if (!std::regex_match(str, unexpectedCharacter))
			throw std::runtime_error("Unexpected Character");
		if (step_mem == step)
			throw std::runtime_error("Unexpected Expression");
	}
	if (step == X)
	{
		step = End;
		multiple = 1;
	}
	if (step == Multiplication)
	{
		step = End;
		exponent = 0;
	}
	std::cout << "end\n multiple: " << multiple << "\n ispositive: " << ispositive << "\n exponent: " << exponent << "\n step: " << step << std::endl;
	if (step != End || exponent == -1 || multiple == -1)
		throw std::runtime_error("Unexpected Expression");
	if (!ispositive)
		multiple = multiple * -1;
	if (_exponent == -1)
		_exponent = exponent;
	else if (_exponent != exponent)
		throw std::runtime_error("error: unexpected Term addtition");
	_value += multiple;
}

Term::Term(std::string newTerm)
{
	ParseAndAdd(newTerm);
}

Term::~Term(){}

int	Term::getExponent()
{
	return _exponent;
}

long	Term::getValue()
{
	return _value;
}