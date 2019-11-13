#ifndef __ExecuteGroup_HPP__
#define __ExecuteGroup_HPP__

#include "executecommand.hpp"

using namespace std;

class ExecuteGroup : public ExecuteCommand {
    private:
	vector<ExecuteCommand*> commands;

    public:
	ExecuteGroup();
	virtual void execute();
	virtual void print_command();
	void add_command(ExecuteCommand* command);
	virtual string get_command();
	virtual string get_command_full();
	virtual string get_separator();
	virtual bool get_success();
};

#endif //__ExecuteGroup_HPP__
