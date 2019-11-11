#ifndef __Execute_HPP__
#define __Execute_HPP__

#include "executecommand.hpp"

using namespace std;

class Execute : public ExecuteCommand {
    private:
	const char* command;
	string separator;
    
    public:
	Execute() : ExecuteCommand() {}
	Execute(const char* enteredCommand, string sep) {
	    this->command = enteredCommand;
	    this->separator = sep;
	}
	virtual void execute() {
	    // Do nothing for now
	}
	virtual void print_command() {
	    cout << command << separator << endl;
	} 
};

#endif //__Execute_HPP__
