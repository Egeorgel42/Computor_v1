#ifndef COMPUTOR_HPP
# define COMPUTOR_HPP
# include <iostream>
# include <regex>
# include <algorithm>
# include <cmath>
# include "Term.hpp"

void	help(void);
void	run(std::string str);
void	calculateFirstDegree(std::vector<Term> &expressions);
void	calculateSecondDegree(std::vector<Term> &expressions);

class	NegativeDiscriminant : public std::exception {const char *what() const throw();};

#endif