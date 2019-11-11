#ifndef __ExecuteGroup_HPP__
#define __ExecuteGroup_HPP__

#include "executecommand.hpp"

using namespace std;

class ExecuteGroup : public ExecuteCommand {
    private:
	vector<ExecuteCommand*> commands;

    public:
	ExecuteGroup() : ExecuteCommand() {};
	virtual void execute() {
	    // Do nothing for now
	}
	virtual void print_command() {
	    for (unsigned int i = 0; i < this->commands.size(); ++i) {
		commands.at(i)->print_command();
	    }
	}
	void add_command(ExecuteCommand* command) {
	    this->commands.push_back(command);
	}
	
};

#endif //__ExecuteGroup_HPP__
