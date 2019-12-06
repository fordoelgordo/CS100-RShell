#ifndef __OUTPUTREDIRECT_HPP__
#define __OUTPUTREDIRECT_HPP__

#include "ExecuteCommand.hpp"

using namespace std;

class OutputRedirect : public ExecuteCommand {

protected:
        ExecuteCommand *left;
	string right;
        //ExecuteCommand *right;

public:
        OutputRedirect() {};
	OutputRedirect(ExecuteCommand *1, string right);
        //OutputRedirect(ExecuteCommand *l, ExecuteCommand *r);
        bool execute();
        void print_command();
        string get_command();
        string get_command_full();
        string get_seperator();

        void set_left(ExecuteCommand *l);
	void set_right(string right);
        //void set_right(ExecuteCommand *r);
}

#endif //__OUTPUTREDIRECT_HPP__

