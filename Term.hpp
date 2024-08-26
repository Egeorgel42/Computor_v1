#ifndef TERM_HPP
# define TERM_HPP
# include <string>
# include <vector>
# include <gmpxx.h>

class Term
{
	public:
		Term(std::string newTerm, bool ispositive);
		Term(int exponent, mpf_class value);
		~Term();
		class		EmptyValue : public std::exception {const char *what() const throw();};
		void		ParseAndAdd(std::string val, bool ispositive);
		Term		&operator+=(const Term& rhs);
		int			getExponent() const;
		mpf_class	getValue() const;
		void		reverseValue();
	private:
		int			_exponent = -1;
		mpf_class	_value = 0;
		Term();

};

std::ostream &operator<<(std::ostream& out, const Term& expression);

enum	Steps
{
	Sign,
	Multiplicator,
	Multiplication,
	X,
	Power,
	Exponent,
	End
};

#endif