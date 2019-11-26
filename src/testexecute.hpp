#ifndef __TestExecute_HPP__
#define __TestExecute_HPP__

#include "executecommand.hpp"
#include <algorithm>
#include <regex>
#include <cctype> // To use isspace command

using namespace std;

class TestExecute : public ExecuteCommand {
    private:
	vector<string> command; // The stat() function can take a string as an argument instead of a char**, so we'll pass in a vector of commands instead
	string separator;
	bool executeSuccess;

    public:
	TestExecute();
	TestExecute(vector<string> enteredCommand, string sep);
	virtual bool execute();
	virtual void print_command();
	virtual string get_command();
	virtual string get_command_full();
	virtual string get_separator();
};

#endif //__TestExecute_HPP__
