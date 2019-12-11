#ifndef __InputRedirect_HPP__
#define __InputRedirect_HPP__

#include "executecommand.hpp"
#include "parser.hpp"

using namespace std;

class InRedirect : public ExecuteCommand {
    private:
	char** command;
	string input;
    
    public:
	InRedirect();
	InRedirect(string command, string input);
	virtual bool execute();
	virtual void print_command();
	virtual string get_command();
	virtual string get_command_full();
	virtual string get_separator();
};

#endif //__InputRedirect_HPP__
