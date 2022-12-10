#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "problem.h"

bool AC_3(Problem& csp);
bool IsPossibleValue(const string& value1,const  set<string>& second_domain,  shared_ptr<Constraint> constraint);
bool ReviseArc(Problem& csp,const pair<string, string>& arc);

#endif