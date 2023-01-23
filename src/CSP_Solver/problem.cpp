#include "problem.h"

void Problem::AddVariable(const string& name, const set<string>& allowed_values){
    
    variableToDomain[name] = allowed_values;
}

void Problem::AddCostraint(const string& var1,const  string& var2, shared_ptr<Constraint> constraint){
    variableToNeigh[var1].insert(var2);
    variableToNeigh[var2].insert(var1);

    arcToConstraint[{var1, var2}] = constraint;
    arcToConstraint[{var2, var1}] = constraint;
}

set<string>& Problem::GetVariableDomain(const string& name){
    return variableToDomain[name];
}

map<string, set<string>>&  Problem::GetVariablesDomain(){
    return variableToDomain;
}

shared_ptr<Constraint> Problem::GetConstraint(const pair<string, string>& arc){
    return arcToConstraint[arc];
}

const map<pair<string, string>, shared_ptr<Constraint>>& Problem::GetArcs(){
    return arcToConstraint;
}

set<string>& Problem::GetNeigh(const string& var){
    return variableToNeigh[var];
}

void Problem::FillArcQueue(queue<pair<string, string>>& queue){

    for(auto& key:arcToConstraint){
        queue.push(key.first);
    }
}

string Problem::toString(){
    string s;
    for(auto& var : variableToDomain){
        s = s + var.first + "  :  ";
        for(auto& value: var.second){
            s = s + value + ",  ";
        }
        s = s + "\n";

    }
    return s;
}

int Problem::domainSize(){
    return domain.size();
}

int Problem::nVariables(){
    return variableToDomain.size();
}

int Problem::constrSize(){
    return arcToConstraint.size();
}