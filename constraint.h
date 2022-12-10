#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <string>
using namespace std;

class Constraint{
    public:
        virtual bool isVerified(const string& val1,const  string& val2) const = 0;
};

class DiffConstraint:public Constraint{
    public:
        bool isVerified (const string& val1,const string& val2) const{return val1 != val2;}
};

class QueenConstraint: public Constraint{
    public:
        QueenConstraint(int _ncol1, int _ncol2): ncol1{_ncol1}, ncol2{_ncol2}{}
        bool isVerified (const string& val1,const string& val2) const{
            int _val1 = val1[0] - '0';
            int _val2 = val2[0] - '0';

            int absdiffvar = ncol1 - ncol2;
            int absdiffval = _val1 - _val2;
            if(absdiffvar < 0) absdiffvar = absdiffvar*(-1);
            if(absdiffval < 0) absdiffval = absdiffval*(-1);

            if(_val1 == _val2) return false;
            if(absdiffvar == absdiffval) return false;
            return true;

        }
    private:
        int ncol1, ncol2;
};

#endif