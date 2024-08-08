#ifndef TERM_HPP
# define TERM_HPP
# include <string>

class Term
{
	public:
		Term(std::string newTerm);
		~Term();
		void	ParseAndAdd(std::string val);
		void	AddParsed(int exponent, long multiple);
		int		getExponent();
		long	getValue();
	private:
		int	_exponent = -1;
		long	_value = 0;
		Term();

};

enum	Steps
{
	Sign,
	Multiplicator,
	Multiplication,
	X,
	End
};

#endif