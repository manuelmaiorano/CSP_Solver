#include <iostream>
#include <memory>
#include "problem.h"
#include "algorithms.h"
#include "solver.h"

bool IsPossibleValue(const string& value1,const  set<string>& second_domain, shared_ptr<Constraint> pconstraint){
    for(auto& value2 : second_domain){
        if(pconstraint->isVerified(value1, value2))  return true;
    }
    return false;
}

bool ReviseArc(Problem& csp, const pair<string, string>& arc){
    bool revised = false;
    set<string>& domain_first = csp.GetVariableDomain(arc.first);
    set<string>& domain_second = csp.GetVariableDomain(arc.second);
    shared_ptr<Constraint> constraint = csp.GetConstraint(arc);

    set<string> domain_first_copy = domain_first;

    for(const string& value: domain_first_copy){
        if(!IsPossibleValue(value, domain_second, constraint)){
            domain_first.erase(value);
            revised = true;
        }
    }
    return revised;
}

bool AC_3(Problem& csp){
    queue<pair<string, string>> arc_queue;
    csp.FillArcQueue(arc_queue);
    while(!arc_queue.empty()){
        pair<string, string> arc = arc_queue.front();
        arc_queue.pop();
        if(ReviseArc(csp, arc)){
            string& Xi = arc.first;
            string& Xj = arc.second;
            if(csp.GetVariableDomain(Xi).empty()) return false;
            for(const string& Xk: csp.GetNeigh(Xi)){
                if(Xk != Xj)  arc_queue.push({Xk, Xi});
            }
            std::cout<<csp.toString()<<std::endl;

        }

    }
    return true;
}

