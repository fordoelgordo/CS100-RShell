#ifndef __INPUTREDIRECT_HPP__
#define __INPUTREDIRECT_HPP__

#include "ExecuteCommand.hpp"

using namespace std;

class InputRedirect : public ExecuteCommand {

protected:
	string left;
        ExecuteCommand *right;

public:
        InputRedirect() {};
        InputRedirect(string left, ExecuteCommand *r);
        bool execute();
        void print_command();
        string get_command();
        string get_command_full();
        string get_seperator();

        void set_left(string left);
        void set_right(ExecuteCommand *r);
}

#endif //__INPUTREDIRECT_HPP__

