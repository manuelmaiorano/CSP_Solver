#include "problem.h"
#include "solver.h"
#include <iostream>
#include <chrono>


Problem getNQueenProblem(int N){

    set<string> allowedvals;
    vector<string> domain;

    for(int i = 1; i <= N; i++){
        char c = i + '0';
        string s{c};
        allowedvals.insert(s);
        domain.push_back(s);
    }
    Problem csp{domain};

    string b = "col";
    for(int i = 1; i <= N; i++){
        char ncolchar = i + '0';
        csp.AddVariable(b += ncolchar, allowedvals);
        b.pop_back();
    }

    for(auto& [var1, dom1]: csp.GetVariablesDomain()){
        for(auto& [var2, dom2]: csp.GetVariablesDomain()){
            int ncol1 = var1.back() - '0';
            int ncol2 = var2.back() - '0';
            if(ncol2 <= ncol1) continue;
            auto pconstr = make_shared<QueenConstraint>(QueenConstraint(ncol1, ncol2));
            csp.AddCostraint(var1, var2, pconstr);
        }
    }
    return csp;
}

void printAssignment(map<string, string> assign){
    for(auto const& [var, value]: assign){
        cout<<var<< " : "<< value<<endl;
    }
}

int main(){

    Problem problem = getNQueenProblem(8);
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

}