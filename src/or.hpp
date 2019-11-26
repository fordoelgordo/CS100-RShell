#ifndef __Or_HPP__
#define __Or_HPP__

#include "executecommand.hpp"

using namespace std;

class Or {
    public:
	Or();
	bool execute(bool leftEval, ExecuteCommand* rightEval);
};

#endif //__Or_HPP__
