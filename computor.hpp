#ifndef COMPUTOR_HPP
# define COMPUTOR_HPP
# include <iostream>
# include <regex>
# include <algorithm>
# include "Term.hpp"

void	help(void);
void	run(std::string str);
void	calculateFirstDegree(std::vector<Term> &expressions);

#endif