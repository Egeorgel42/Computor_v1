#include "computor.hpp"

const char      *Term::EmptyValue::what() const throw() {return "0";};

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

static void parseSign(std::string str, uint &step, uint &current_step, bool &ispositive, mpf_class &value)
{
	convetionalSignParsing(str, step, current_step, Sign, Multiplicator, "+", false, false);
	ispositive = ispositive == !convetionalSignParsing(str, step, current_step, Sign, Multiplicator, "-", false, false);
	if (current_step == Sign && step == Sign)
		step = current_step = Multiplicator;
	if (convetionalSignParsing(str, step, current_step, Multiplicator, X, "xX", false, true))
	{
		value = 1;
		step = current_step = X;
	}
	if (convetionalSignParsing(str, step, current_step, Multiplication, X, "xX", false, true))
		step = current_step = X;
	convetionalSignParsing(str, step, current_step, Multiplication, X, "*", false, false);
	convetionalSignParsing(str, step, current_step, X, Power, "xX", true, false);
	convetionalSignParsing(str, step, current_step, Power, Exponent, "^", true, false);
}

static void	parseValue(std::string str, uint &step, uint &current_step, mpf_class &value, uint expected_step, uint following_step)
{
	std::regex nbr = std::regex(R"(\d+)");
	if (current_step == expected_step && std::regex_match(str, nbr))
	{
		try {
			std::stod(str);
		}
		catch (std::out_of_range &e) {
			throw std::runtime_error("Int Overflow");
		}
		value.set_str(str, 10);
		step = following_step;
	}
	else if (current_step == expected_step)
		throw std::runtime_error("Expected Numerical Expression, Instead: " + str);
}

static void	parseExponent(std::string str, uint &step, uint &current_step, int &value, uint expected_step, uint following_step)
{
	std::regex nbr = std::regex(R"(\d+)");
	if (current_step == expected_step && std::regex_match(str, nbr))
	{
		try {
			value = std::stoi(str);
		}
		catch (std::out_of_range &e) {
			throw std::runtime_error("Int Overflow");
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
	std::regex split = std::regex(R"([\+\-\*\^Xx]|[^\+\*\-\^Xx\s]+)");
	std::regex unexpectedCharacter = std::regex(R"(([xX\-\+\^\*]|[0-9])*)");
	std::regex_token_iterator<std::string::iterator> it(val.begin(), val.end(), split);
	std::regex_token_iterator<std::string::iterator> end;
	_exponent = -1;
	_value = -1;

	if (it == end)
		throw EmptyValue();
	for (; it != end; it++)
	{
		current_step = step;
		if (step == End)
			throw std::runtime_error("Unexpected Expression");
		if (!std::regex_match((std::string)*it, unexpectedCharacter))
			throw std::runtime_error("Unexpected Character, Expressions must only contain (0-9,x,X,-,+,*,^)");
		parseSign(*it, step, current_step, ispositive, _value);
		parseValue(*it, step, current_step, _value, Multiplicator, Multiplication);
		parseExponent(*it, step, current_step, _exponent, Exponent, End);
		if (current_step == step)
			throw std::runtime_error("Unexpected Expression");
	}

	if (_exponent > 2)
		throw std::runtime_error("The polynomial degree is strictly greater than 2, I can't solve");
	if (current_step == Multiplicator)
		_exponent = 0;
	else if (current_step == X)
		_exponent = 1;
	else if (step != End || _exponent == -1 || _value == -1)
		throw std::runtime_error("Unexpected Expression");

	if (_value == 0)
		throw EmptyValue();
	if (!ispositive)
		_value = _value * -1;
}

Term::Term(std::string newTerm, bool ispositive)
{
	ParseAndAdd(newTerm, ispositive);
}


Term::Term(int exponent, mpf_class value) : _exponent(exponent), _value(value) {}

Term::~Term(){}

Term	&Term::operator+=(const Term& rhs)
{
	if (_exponent != rhs._exponent)
		throw std::runtime_error("error: unexpected Term addition");
	_value += _value;
	if (_value == 0)
		throw EmptyValue();
	return *this;
}


std::ostream &operator<<(std::ostream& out, const Term& expression)
{
	if (expression.getValue() != 1 && expression.getValue() > 0)
		out << expression.getValue();
	else if (expression.getValue() != -1 && expression.getValue() < 0)
		out << expression.getValue() * -1;
	if (expression.getExponent() == 1)
		out << "X ";
	else if (expression.getExponent() == 2)
		out << "X^2 ";
	else if (expression.getValue() == 1 || expression.getValue() == -1)
		out << "1 ";
	else
		out << " ";
	return out;
}

void	Term::reverseValue()
{
	_value = _value * - 1;
}

int	Term::getExponent() const
{
	return _exponent;
}

mpf_class	Term::getValue() const
{
	return _value;
}