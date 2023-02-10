#include <iostream>
#include <thread>
#include "solver.h"
#include "problem.h"
#include "algorithms.h"
#include "ThreadPool.h"

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
    if (callback){
        callback(assignment.size());
    }
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


map<string, string>& Solver::parallel_solve(Problem& csp, function<void(int)> callback){
    stack<FrontierNode> initial_frontier;
    map<string, string> empty_assignment;
    auto var = selectUnassigned(csp.GetVariablesDomain(), csp.domainSize(), empty_assignment);
    for (auto& val: csp.GetVariableDomain(var)){
        FrontierNode node;
        map<string, set<string>> domainCopy = csp.GetVariablesDomain();
        map<string, string> new_assigment;
        domainCopy[var] = {val};
        new_assigment[var] = val;
        MAC(csp, domainCopy, var);
        node.partial_assignment = new_assigment;
        node.domain = domainCopy;
        initial_frontier.push(node);
    }
    pool.Start();

    pool.QueueJob([initial_frontier, this, &csp](){
        this->searchParallel(csp, initial_frontier);
    });

    unique_lock lk(doneVariableMutex);
    this->conditionOnDone.wait(lk, [this]{return this->done;});
    lk.unlock();

    pool.Stop();
    return this->assignment;

}

void Solver::searchParallel(Problem& csp, stack<FrontierNode> frontier){
    while (!frontier.empty()){
        FrontierNode node = frontier.top();
        frontier.pop();
        if (node.partial_assignment.size() == csp.nVariables()){
            {
                lock_guard lk(this->doneVariableMutex);
                done = true;
                this->assignment = node.partial_assignment;
            }
            this->conditionOnDone.notify_all();
            return;
        }
        if (frontier.size() > MAX_FRONTIER_SIZE_PER_JOB){
            {
                lock_guard lk(doneVariableMutex);
                if(done){
                    return;
                }
            }
            stack<FrontierNode> newFrontier;
            while(frontier.size() > MAX_FRONTIER_SIZE_PER_JOB/2){
                FrontierNode node = frontier.top();
                frontier.pop();
                newFrontier.push(node);
            }
            pool.QueueJob([newFrontier, this, &csp]{
                this->searchParallel(csp, newFrontier);
            });
        }
        auto var = selectUnassigned(node.domain, csp.domainSize(), node.partial_assignment);
        for(auto& val: node.domain[var]){
            map<string, set<string>> domainCopy = node.domain;
            domainCopy[var] = {val};
            map<string, string> assignmentCopy = node.partial_assignment;
            assignmentCopy[var] = val;
            bool isValid = MAC(csp, domainCopy, var);
            if(!isValid) {
                continue;
            }
            FrontierNode new_node;
            new_node.partial_assignment = assignmentCopy;
            new_node.domain = domainCopy;
            frontier.push(new_node);
        }


    }

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