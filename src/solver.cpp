#include <iostream>
#include "solver.h"
#include "problem.h"
#include "algorithms.h"

bool ReviseDomain(Problem& csp, const pair<string, string>& arc, 
                set<string>& domain_first, set<string>& domain_second){

    bool revised = false;
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

bool MAC(Problem& csp, map<string, set<string>>& domain, string& assignedVar){

    queue<pair<string, string>>  starting_queue;
    for(auto& neigh: csp.GetNeigh(assignedVar)){
        if(domain[neigh].size() > 1)
            starting_queue.push({neigh, assignedVar});
    }
    
    while(!starting_queue.empty()){
        pair<string, string> arc = starting_queue.front();
        starting_queue.pop();
        string& Xi = arc.first;
        string& Xj = arc.second;
        if(ReviseDomain(csp, arc, domain[Xi], domain[Xj])){
            if(domain[Xi].empty()) return false;
            for(const string& Xk: csp.GetNeigh(Xi)){
                if(Xk != Xj)  starting_queue.push({Xk, Xi});
            }
        }
    }

    return true;

 }

string selectUnassigned(map<string, set<string>>& currDomain, int domainSize, map<string, string> assignment){
    int min_size = domainSize;
    map<string, set<string>>::iterator it = currDomain.begin();
    string currVar = it->first;
    for(auto& [var,dom]: currDomain){
        int vecsize = dom.size();
        if(vecsize <= min_size && assignment.count(var) == 0){
            min_size = vecsize;
            currVar = var;
        }
    }
    return currVar;
}

bool Solver::BackTrack(Problem& csp, map<string, set<string>>& currDomain, function<void(int)> callback){
    if(assignment.size() == csp.nVariables()) return true;
    callback(assignment.size());
    string var = selectUnassigned(currDomain, csp.domainSize(), assignment);
    for(auto& value: currDomain[var]){
        map<string, set<string>> domainCopy = currDomain;
        domainCopy[var] = {value};
        assignment[var] = value;
        bool isValid = MAC(csp, domainCopy, var);
        if(!isValid) {
            assignment.erase(var);
            continue;
        }
        if(BackTrack(csp, domainCopy, callback))  return true;
    }
    assignment.erase(var);
    return false;
}

map<string, string>& Solver::solve(Problem& csp, function<void(int)> callback){
    if(!BackTrack(csp, csp.GetVariablesDomain(), callback)) throw "cannot solve";
    return assignment;
}

string Solver::toString(){
    string s;
    for(auto& elem: assignment){
        s = s + elem.first + " : " + elem.second + "\n";
    }
    return s;
}

map<string, string>& Solver::GetAssignment(){
    return assignment;
}

void printSudoku(map<string, string> assignment){
    string b = "V";
    for(int i = 1; i<=9; i++){
        for(int j = 1; j<=9; j++){
            char ic = i + '0';
            char jc = j + '0';
            b+=ic; b+=jc;
            if(assignment.count(b) > 0){
                cout<< assignment[b] +" ";
            }
            else{
                cout<<"0 ";
            }
            b.pop_back(); b.pop_back();
        }
        cout<< endl;
    
    }
    cout << endl;
}