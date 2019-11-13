#include "executegroup.hpp"

using namespace std;

ExecuteGroup::ExecuteGroup() : ExecuteCommand() {}
void ExecuteGroup::execute() {
    // The first command should be executed no matter what
    if (this->commands.at(0)->get_command() == "exit") {
	exit(0);
    }
    else {
	this->commands.at(0)->execute();
    }
    // Loop through the rest of the commands, applying connector logic
    for (int i = 1; i < commands.size(); ++i) {
	if (this->commands.at(i - 1)->get_separator() == ";") { // If a command is followed by ; then the command after it is always executed
	    if (this->commands.at(i)->get_command() == "exit") {
		exit(0);
	    }
	    else {
		this->commands.at(i)->execute();
	    }
	}
	else if (this->commands.at(i - 1)->get_separator() == "&&") { // If a command is followed by && then the command after it is executed only if the first one succeeds
	    if (this->commands.at(i - 1)->get_success()) {
		if (this->commands.at(i)->get_command() == "exit") {
		    exit(0);
		}
		else {
		    this->commands.at(i)->execute();
		}
	    }
	    else {
		++i; // skip current command in chain and go to next one
	    }
	}
	else if (this->commands.at(i - 1)->get_separator() == "||") { // If a command is followed by || then the command after it is executed only if the first one failed
	    if (!this->commands.at(i - 1)->get_success()) {
		if (this->commands.at(i)->get_command() == "exit") {
		    exit(0);
		}
		else {
		    this->commands.at(i)->execute();
		}
	    }
	    else {
		++i; // Skip the current command in the chain and go to the next one
	    }
	}
	else { // If this branch of the conditional is reached, then the command contains an invalid separator
	    cout << "Invalid separator in command" << endl;
	    exit(1); // Exit with error status 1
	}    	    
    }
    
}
void ExecuteGroup::print_command() {
    cout << "************************" << endl;
    cout << "PRINTING COMMANDS OBJECT" << endl;
    for (unsigned int i = 0; i < this->commands.size(); ++i) {
	commands.at(i)->print_command();
    }
    cout << "************************" << endl;
}
void ExecuteGroup::add_command(ExecuteCommand* command) {
    this->commands.push_back(command);
}
string ExecuteGroup::get_command() { /*Do nothing in this class' implementation */}
string ExecuteGroup::get_command_full() {
    string full;
    for (unsigned int i = 0; i < this->commands.size(); ++i) {
	full += this->commands.at(i)->get_command_full();
    }

    return full;

}
string ExecuteGroup::get_separator() { /*Do nothing in this class' implementation */}
bool ExecuteGroup::get_success() { /*Do nothing in this class' implementation */}

