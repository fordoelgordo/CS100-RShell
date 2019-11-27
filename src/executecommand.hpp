#ifndef __ExecuteCommand_HPP__
#define __ExecuteCommand_HPP__

#include <vector>
#include <string>
#include <iostream>
#include <unistd.h> // To use the execvp() function
#include <sys/wait.h> // To execute waitpid() command which allows the parent process to wait for the child
#include <sys/stat.h> // To execute stat() command which is used to check if a file path exists or not
#include <cstring> // To use strcpy command

using namespace std;

class ExecuteCommand {
    public:
	ExecuteCommand() {};
	virtual bool execute() = 0;
	virtual void print_command() = 0;
	virtual string get_command() = 0;
	virtual string get_command_full() = 0;
	virtual string get_separator() = 0;	
};

#endif //__ExecuteCommand_HPP__
