#ifndef __ExecuteCommand_HPP__
#define __ExecuteCommand_HPP__

#include <vector>
#include <string>
#include <iostream>

class ExecuteCommand {
    public:
	ExecuteCommand() {};
	virtual void execute() = 0;
	virtual void print_command() = 0;
};

#endif //__ExecuteCommand_HPP__
