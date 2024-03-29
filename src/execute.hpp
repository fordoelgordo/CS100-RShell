#ifndef __Execute_HPP__
#define __Execute_HPP__

#include "executecommand.hpp"

using namespace std;

class Execute : public ExecuteCommand {
    private:
	char** command;
	string separator;
	bool executeSuccess;
    
    public:
	Execute();
	Execute(char** enteredCommand, string sep);
	virtual bool execute();
	virtual void print_command();
	virtual string get_command();
	virtual string get_command_full();
	virtual string get_separator();
};

#endif //__Execute_HPP__
