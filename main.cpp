#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "problem.h"
#include "algorithms.h"
#include "constraint.h"
#include "solver.h"

static auto diffConst = make_shared<DiffConstraint>(DiffConstraint());

void addAllDiff(Problem& csp, vector<string>& vars){
    for(int idx1 = 0; idx1 <= 8; idx1++){
        for(int idx2 = 0; idx2 <= 8; idx2++){
            if(idx2 <= idx1) continue;
            csp.AddCostraint(vars[idx1], vars[idx2], diffConst);
        }
    }
}

void addRowConstr(Problem& csp, vector<vector<string>>& vars, int row){
    for(int col1 = 0; col1 <= 8; col1++){
        for(int col2 = 0; col2 <= 8; col2++){
            if(col2 <= col1) continue;
            csp.AddCostraint(vars[row][col1], vars[row][col2], diffConst);
        }
    }
}

void addColumnConstr(Problem& csp, vector<vector<string>>& vars, int col){
    for(int row1 = 0; row1 <= 8; row1++){
        for(int row2 = 0; row2 <= 8; row2++){
            if(row2 <= row1) continue;
            csp.AddCostraint(vars[row1][col], vars[row2][col], diffConst);
        }
    }   
}

void addBlkConstr(Problem& csp, vector<vector<string>>& vars, int row, int col){
    vector<string> blkvars;
    int offsetrow = 3 * row;
    int offsetcol = 3 * col;
    for(int blkrow = 0; blkrow <= 2; blkrow++){
        for(int blkcol = 0; blkcol <= 2; blkcol++){
            blkvars.push_back(vars[blkrow +offsetrow][blkcol+offsetcol]);
        }
    }
    addAllDiff(csp, blkvars);

}

Problem getSudoku(map<string, string>& fixed){
    set<string> allowedvals;
    vector<string> domain;

    for(int i = 1; i <= 9; i++){
        char c = i + '0';
        string s{c};
        allowedvals.insert(s);
        domain.push_back(s);
    }
    Problem csp{domain};

    int sz;

    string b = "V";
    vector<vector<string>> vars;
    for(int i = 1; i <= 9; i++){
        vector<string> row;
        vars.push_back(row);
        for(int j = 1; j <= 9; j++){
            char ic = i + '0';
            char jc = j + '0';
            b+=ic; b+=jc;
            vars[i-1].push_back(b);
            b.pop_back(); b.pop_back();
        }
    }

    for(int row = 0; row <= 8; row++){
        for(int col = 0; col <= 8; col++){
            string var = vars[row][col];
            if(fixed.count(var) != 0){
                csp.AddVariable(var, {fixed[var]});
                continue;
            }
            csp.AddVariable(var, allowedvals);
        }
    }

    sz = csp.constrSize();

    for(int row = 0; row <= 8; row++){
        addRowConstr(csp, vars, row);
    }

    sz = csp.constrSize();

    for(int col = 0; col <= 8; col++){
        addColumnConstr(csp, vars, col);
    }

    sz = csp.constrSize();

    for(int row = 0; row <= 2; row++){
        for(int col = 0; col <= 2; col++){
            addBlkConstr(csp, vars, row, col);
        }
    }

    sz = csp.constrSize();

    return csp;
}

Problem getSudokuFromFile(string filename){
    map<string, string> fixed;
    string line;
    int curr_row = 1;
    int curr_col = 1;
    ifstream file{filename};

    string b = "V";
    if(file.is_open()){
        while(getline(file, line)){
            for(auto c: line){
                if(c != '0'){
                    char c_col = curr_col + '0';
                    char c_row = curr_row + '0';
                    b += c_row; b+= c_col;
                    fixed[b] = c;
                    b.pop_back(); b.pop_back();
                }
                curr_col += 1;
            }
            curr_row += 1;
            curr_col = 1;
        }
    }

    return getSudoku(fixed);

}

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

void test_AC3(){
    Problem csp = getAustraliaProblem();

    AC_3(csp);
    cout << csp.toString();
}

void test_RA(){

    Problem csp = getAustraliaProblem();

    ReviseArc(csp, {"NT", "WA"});
    cout<<csp.toString();
}

void test_IPV(){
    DiffConstraint c = DiffConstraint();
    shared_ptr<Constraint> ptr(&c);
    bool p = IsPossibleValue("value1", {"value1"}, ptr);
    string s = p ? "true" : "false";
    cout << s;

}

void test_const(){
    DiffConstraint constr = DiffConstraint();
    cout<< constr.isVerified("val1", "val2");
    cout<< constr.isVerified("val1", "val1");
}

void test_BT(Problem& csp){
    Solver solver;
    int n_var = csp.nVariables();
    int lastp = -1;
    string s = "percentage: ";

    try{
        solver.solve(csp, [n_var, &lastp, &s](int size_so_far){ 
            int p = size_so_far *100/n_var;
            if(p > lastp){
                cout<< "\r" << s << p << "%" << flush;
                lastp = p;
            }
        });
        cout << "\r" << s << "100%" << endl;
        //cout << solver.toString();
        printSudoku(solver.GetAssignment());
    }
    catch(const char* msg){
        cout << msg;
    }
    
}

int main(){


    Problem csp = getSudokuFromFile("files/sud.txt");
    //Problem csp = getAustraliaProblem();
    //Problem csp = getNQueenProblem(8);
    int sz = csp.constrSize();
    test_BT(csp);

}