#ifndef PROBLEM_H
#define PROBLEM_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <memory>
#include "constraint.h"

using namespace std;

class Problem{
    public:
        Problem(vector<string> _domain): domain{_domain}{}
        void AddVariable(const string& name, const set<string>& allowed_values);
        void AddCostraint(const string& var1,const  string& var2, shared_ptr<Constraint> constraint);
        set<string>& GetVariableDomain(const string& name);
        map<string, set<string>>&    GetVariablesDomain();
        shared_ptr<Constraint> GetConstraint(const pair<string, string>& arc);
        const map<pair<string, string>, shared_ptr<Constraint>>& GetArcs();
        set<string>& GetNeigh(const string& var);
        void FillArcQueue(queue<pair<string, string>>& queue);
        string toString();
        int nVariables();
        int domainSize();
        int constrSize();
    private:
        vector<string>    domain;
        map<string, set<string>>     variableToDomain;
        map<string, set<string>>     variableToNeigh;
        map<pair<string, string>, shared_ptr<Constraint>>       arcToConstraint;
};

#endif