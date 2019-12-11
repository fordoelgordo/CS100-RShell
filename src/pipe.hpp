#ifndef __Pipe_HPP__
#define __Pipe_HPP__

#include "executecommand.hpp"
#include "outputredirect.hpp"
#include "inputredirect.hpp"
#include "parser.hpp"
#include <queue>

using namespace std;

class Pipe : public ExecuteCommand {
    private:
	vector<string> commands;

    public:
	Pipe();
	Pipe(vector<string> commands);
	virtual bool execute();
	virtual void print_command();
	virtual string get_command();
	virtual string get_command_full();
	virtual string get_separator();
};

#endif //__Pipe_HPP__
