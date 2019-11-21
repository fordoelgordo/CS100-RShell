#ifndef __TestExecute_HPP__
#define __TestExecute_HPP__

#include "executecommand.hpp"

using namespace std;

class TestExecute : public ExecuteCommand {
    private:
	vector<string> command; // The stat() function can take a string as an argument instead of a char**, so we'll pass in a vector of commands instead
	string separator;
	bool executeSuccess;

    public:
	TestExecute();
	TestExecute(vector<string> enteredCommand, string sep);
	virtual void execute();
	virtual void print_command();
	virtual string get_command();
	virtual string get_command_full();
	virtual string get_separator();
	virtual bool get_success();
};

#endif //__TestExecute_HPP__
