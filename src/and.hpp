#ifndef __And_HPP__
#define __And_HPP__

#include "executecommand.hpp"

using namespace std;

class And {
    public:
	And();
	bool execute(bool leftEval, ExecuteCommand* rightEval);
};

#endif //__And_HPP__
