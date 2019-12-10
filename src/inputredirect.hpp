#ifndef __InputRedirect_HPP__
#define __InputRedirect_HPP__

#include "executecommand.hpp"

using namespace std;

class InRedirect : public ExecuteCommand {
    private:
	ExecuteCommand* input;
	string output;
    
    public:
	InRedirect();
	InRedirect(ExecuteCommand* input, string output);
	virtual bool execute();
	virtual void print_command();
	virtual string get_command();
	virtual string get_command_full();
	virtual string get_separator();
};

#endif //__InputRedirect_HPP__
