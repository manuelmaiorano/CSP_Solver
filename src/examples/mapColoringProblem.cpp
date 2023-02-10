#include <problem.h>
#include <iostream>
#include "constraint.h"
#include "solver.h"
#include <chrono>

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

void printAssignment(map<string, string> assign){
    for(auto const& [var, value]: assign){
        cout<<var<< " : "<< value<<endl;
    }
}

int main(){

    Problem problem = getAustraliaProblem();
    Solver solver;
    auto t1 = chrono::high_resolution_clock::now();
    solver.solve(problem, NULL);
    auto t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds> (t2 -t1).count();
    cout<<"elapsed : " << duration<< endl;
    auto assignment = solver.GetAssignment();
    printAssignment(assignment);

    cout<<" doing parallel now"<< endl;

    Solver solver1;
    t1 = chrono::high_resolution_clock::now();
    solver1.parallel_solve(problem, NULL);
    t2 = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds> (t2 -t1).count();
    cout<<"elapsed : " << duration<< endl;
    assignment = solver1.GetAssignment();
    printAssignment(assignment);

    return 0;

}