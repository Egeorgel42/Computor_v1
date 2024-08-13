#include "computor.hpp"

const char      *Term::EmptyValue::what() const throw() {return "0";};

static long checkOverflow(long a, long b)
{
	long c = a + b;
	if ((a > 0) == (b > 0) && (c > 0) != (a > 0))
		throw std::runtime_error("Long Overflow");
	return c;
}

static bool convetionalSignParsing(std::string str, uint &step, uint &current_step, uint expected_step, uint following_step, std::string expected_character, bool expected, bool exception)
{
	if (current_step == expected_step)
	{
		if (expected_character.find(str) != std::string::npos)
		{
			step = following_step;
			return true;
		}
		else if (expected)
			throw std::runtime_error("Expected Sign/Operator/Character(s): " + expected_character + ", Instead: " + str);
	}
	else if (!exception && expected_character.find(str) != std::string::npos)
		throw std::runtime_error("Unexpected Sign/Operator/Character: " + str);
	return false;
}

static void parseSign(std::string str, uint &step, uint &current_step, bool &ispositive, long &multiple)
{
	convetionalSignParsing(str, step, current_step, Sign, Multiplicator, "+", false, false);
	ispositive = ispositive == !convetionalSignParsing(str, step, current_step, Sign, Multiplicator, "-", false, false);
	if (current_step == Sign && step == Sign)
		step = current_step = Multiplicator;
	if (convetionalSignParsing(str, step, current_step, Multiplicator, X, "xX", false, true))
	{
		multiple = 1;
		step = current_step = X;
	}
	if (convetionalSignParsing(str, step, current_step, Multiplication, X, "xX", false, true))
		step = current_step = X;
	convetionalSignParsing(str, step, current_step, Multiplication, X, "*", false, false);
	convetionalSignParsing(str, step, current_step, X, Power, "xX", true, false);
	convetionalSignParsing(str, step, current_step, Power, Exponent, "^", true, false);
}

static void	parseNbr(std::string str, uint &step, uint &current_step, long &nbr, uint expected_step, uint following_step)
{
	std::regex multiplicator = std::regex(R"(\d+)");
	if (current_step == expected_step && std::regex_match(str, multiplicator))
	{
		try {
			nbr = std::stol(str);
		}
		catch (std::out_of_range &e) {
			throw std::runtime_error("Long Overflow");
		}
		step = following_step;
	}
	else if (current_step == expected_step)
		throw std::runtime_error("Expected Numerical Expression, Instead: " + str);
}

void	Term::ParseAndAdd(std::string val, bool ispositive)
{
	uint step = Sign;
	uint current_step;
	long exponent = -1;
	long multiple = -1;
	std::regex split = std::regex(R"([\+\-\*\^Xx]|[^\+\*\-\^Xx\s]+)");
	std::regex unexpectedCharacter = std::regex(R"(([xX\-\+\^\*]|[0-9])*)");
	std::regex_token_iterator<std::string::iterator> it(val.begin(), val.end(), split);
	std::regex_token_iterator<std::string::iterator> end;

	if (it == end)
		throw std::runtime_error("Empty value");
	for (; it != end; it++)
	{
		current_step = step;
		if (step == End)
			throw std::runtime_error("Unexpected Expression");
		if (!std::regex_match((std::string)*it, unexpectedCharacter))
			throw std::runtime_error("Unexpected Character, Expressions must only contain (0-9,x,X,-,+,*,^)");
		parseSign(*it, step, current_step, ispositive, multiple);
		parseNbr(*it, step, current_step, multiple, Multiplicator, Multiplication);
		parseNbr(*it, step, current_step, exponent, Exponent, End);
		if (current_step == step)
			throw std::runtime_error("Unexpected Expression");
	}

	if (exponent > 2)
		throw std::runtime_error("Exponent must be between 0 and 2");
	if (current_step == Multiplicator)
		exponent = 0;
	else if (current_step == X)
		exponent = 1;
	else if (step != End || exponent == -1 || multiple == -1)
		throw std::runtime_error("Unexpected Expression");

	if (exponent == 0 && multiple == 0)
		throw Term::EmptyValue();
	if (!ispositive)
		multiple = multiple * -1;
	if (_exponent == -1)
		_exponent = exponent;
	else if (_exponent != exponent)
		throw std::runtime_error("error: unexpected Term addtition");
	_value = checkOverflow(_value, multiple);
}

Term::Term(std::string newTerm, bool ispositive)
{
	ParseAndAdd(newTerm, ispositive);
}

Term::~Term(){}

Term	&Term::operator+=(const Term& rhs)
{
	if (_exponent != rhs._exponent)
		throw std::runtime_error("error: unexpected Term addition");
	_value = checkOverflow(_value, rhs._value);
	return *this;
}

int	Term::getExponent()
{
	return _exponent;
}

long	Term::getValue()
{
	return _value;
}