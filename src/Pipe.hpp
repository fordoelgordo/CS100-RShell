#ifndef __PIPE_HPP__
#define __PIPE_HPP__

#include "ExecuteCommand.hpp"

using namespace std;
	
class Pipe : public ExecuteCommand {

protected:
        string left;
        ExecuteCommand *right;

public:
        Pipe() {};
        Pipe(string left, ExecuteCommand *r);
        bool execute();
        void print_command();
        string get_command();
        string get_command_full();
        string get_seperator();

        void set_left(string left);
        void set_right(ExecuteCommand *r);
}

#endif //__PIPE_HPP__

