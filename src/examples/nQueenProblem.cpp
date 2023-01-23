#include <problem.h>


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

    for(auto& [var1,_]: csp.GetVariablesDomain()){
        for(auto& [var2,_]: csp.GetVariablesDomain()){
            int ncol1 = var1.back() - '0';
            int ncol2 = var2.back() - '0';
            if(ncol2 <= ncol1) continue;
            auto pconstr = make_shared<QueenConstraint>(QueenConstraint(ncol1, ncol2));
            csp.AddCostraint(var1, var2, pconstr);
        }
    }
    return csp;
}