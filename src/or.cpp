#include "or.hpp"

using namespace std;

// Constructor
Or::Or() {}

// execute()
bool Or::execute(bool leftEval, ExecuteCommand* rightEval) {
    // Only execute if leftEval fails
    if (!leftEval) {
	return rightEval->execute();
    }

    return false;
}
