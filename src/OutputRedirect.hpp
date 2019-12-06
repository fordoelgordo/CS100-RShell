#ifndef __OUTPUTREDIRECT_HPP__
#define __OUTPUTREDIRECT_HPP__

#include "ExecuteCommand.hpp"

using namespace std;

class OutputRedirect : public ExecuteCommand {

protected:
	string left;
        ExecuteCommand *right;

public:
        OutputRedirect() {};
	OutputRedirect(string left, ExecuteCommand *r);
        bool execute();
        void print_command();
        string get_command();
        string get_command_full();
        string get_seperator();

        void set_left(string left);
        void set_right(ExecuteCommand *r);
}

#endif //__OUTPUTREDIRECT_HPP__

