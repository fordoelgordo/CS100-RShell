#include "factory.hpp"

using namespace std;

// Constructor
Factory::Factory() {}

// Create appropriate command object
ExecuteCommand* Factory::create_command(string command, string sep) {
    ExecuteCommand* com = nullptr;

    if (command.substr(0,4) == "test" || command.substr(0,1) == "[") {
	 com = new TestExecute(create_stringvec(command), sep);
    }
    else if (command == "exit") {
	 com = new Exit();
    }
    else {
	 com = new Execute(create_charstar(command), sep);
    }

    return com;
}	
