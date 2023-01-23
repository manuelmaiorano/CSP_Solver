#include <problem.h>
#include <iostream>
#include "constraint.h"
#include "solver.h"

static auto diffConst = make_shared<DiffConstraint>(DiffConstraint());

Problem getAustraliaProblem(){
    Problem csp{{"red", "green", "blue"}};
    csp.AddVariable("WA", {"red", "green", "blue"});
    csp.AddVariable("NT", {"red", "green", "blue"});
    csp.AddVariable("SA", {"red", "green", "blue"});
    csp.AddVariable("Q", {"red", "green", "blue"});
    csp.AddVariable("NSW", {"red", "green", "blue"});
    csp.AddVariable("V", {"red", "green", "blue"});
    csp.AddVariable("T", {"red", "green", "blue"});

    csp.AddCostraint("WA", "NT", diffConst);
    csp.AddCostraint("WA", "SA", diffConst);
    csp.AddCostraint("NT", "SA", diffConst);
    csp.AddCostraint("NT", "Q", diffConst);
    csp.AddCostraint("SA", "Q", diffConst );
    csp.AddCostraint("Q", "NSW",  diffConst);
    csp.AddCostraint("SA", "NSW",  diffConst);
    csp.AddCostraint("SA", "V",  diffConst);
    csp.AddCostraint("V", "NSW",  diffConst);

    return csp;
}

void callback(int percentage){
    return;
}

void printAssignment(map<string, string> assign){
    for(auto const& [var, value]: assign){
        cout<<var<< " : "<< value<<endl;
    }
}

int main(){

    Problem problem = getAustraliaProblem();
    Solver solver;
    solver.solve(problem, &callback);
    auto assignment = solver.GetAssignment();
    printAssignment(assignment);
    return 0;

}