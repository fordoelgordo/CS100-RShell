#ifndef __Exit_HPP__
#define __Exit_HPP__

#include "executecommand.hpp"

using namespace std;

class Exit : public ExecuteCommand {
    public:
	Exit();
	virtual bool execute();
	virtual void print_command();
	virtual string get_command();
	virtual string get_command_full();
	virtual string get_separator();
};

#endif //__Exit_HPP__
