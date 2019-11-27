#ifndef __Semi_HPP__
#define __Semi_HPP__

#include "executecommand.hpp"

using namespace std;

class Semi {
    public:
	Semi();
	bool execute(bool leftEval, ExecuteCommand* rightEval);
};

#endif //__Semi_HPP__
