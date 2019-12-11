#ifndef __OutputRedirect_HPP__
#define __OutputRedirect_HPP__

#include "executecommand.hpp"
#include "parser.hpp"

using namespace std;

class OutRedirect : public ExecuteCommand {
    private:
	char** command;
	string output;

    public:
	OutRedirect();
	OutRedirect(string input, string output);
	virtual bool execute();
	virtual void print_command();
	virtual string get_command();
	virtual string get_command_full();
	virtual string get_separator();
};

#endif //__OutputRedirect_HPP__
