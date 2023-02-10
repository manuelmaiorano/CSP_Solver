#include <string>
#include <map>
#include <functional>
#include <mutex>
#include <condition_variable>
#include "problem.h"
#include "ThreadPool.h"
#include <stack>


using namespace std;

const int MAX_FRONTIER_SIZE_PER_JOB = 10;

struct FrontierNode {
    map<string, string> partial_assignment;
    map<string, set<string>> domain;
};

class Solver{
    public:
        map<string, string>& solve(Problem& csp, function<void(int)> callback);
        map<string, string>& parallel_solve(Problem& csp, function<void(int)> callback);
        void searchParallel(Problem& csp, stack<FrontierNode> frontier);
        string toString();
        map<string, string>& GetAssignment();
    private:
        bool BackTrack(Problem& csp, map<string, set<string>>& currDomain, function<void(int)> callback);
        map<string, string> assignment;
        mutex doneVariableMutex;
        condition_variable conditionOnDone;
        bool done = false;
        ThreadPool pool;

};
