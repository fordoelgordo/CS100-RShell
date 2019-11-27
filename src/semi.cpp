#include "semi.hpp"

using namespace std;

// Constructor
Semi::Semi() {}

// execute()
bool Semi::execute(bool leftEval, ExecuteCommand* rightEval) {
    // Execute right operand no matter what
    return rightEval->execute();
}
