#include "and.hpp"

using namespace std;

// Constructor
And::And() {}

// execute()
bool And::execute(bool leftEval, ExecuteCommand* rightEval) {
    // Only execute if leftEval is true
    if (leftEval) {
	return rightEval->execute();
    }
    
    return false;
}
