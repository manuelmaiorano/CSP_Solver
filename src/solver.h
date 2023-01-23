#include <string>
#include <map>
#include <functional>
#include "problem.h"


using namespace std;


class Solver{
    public:
        map<string, string>& solve(Problem& csp, function<void(int)> callback);
        string toString();
        map<string, string>& GetAssignment();
    private:
        bool BackTrack(Problem& csp, map<string, set<string>>& currDomain, function<void(int)> callback);
        map<string, string> assignment;

};

void printSudoku(map<string, string> assignment);