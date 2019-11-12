#ifndef __Execute_HPP__
#define __Execute_HPP__

#include "executecommand.hpp"

using namespace std;

class Execute : public ExecuteCommand {
    private:
	char** command;
	string separator;
	bool executeSuccess;
    
    public:
	Execute() : ExecuteCommand() {this->executeSuccess = false;}
	Execute(char** enteredCommand, string sep) {
	    this->command = enteredCommand;
	    this->separator = sep;
	}
	virtual void execute() {
	    // Implement the command execution logic
	    pid_t childPid;
	    pid_t pid;
	    int status;
	    childPid = fork();

	    if (childPid == 0) {
		// In the child process, execute commands using execvp()
		string cmd = *this->command;
		if (cmd == "exit") { // If an exit command is entered, exit the child process
		    _Exit(0); // Exit entire program if exit command is entered
		}
		else {
		    if(execvp(this->command[0], this->command) < 0) {
			perror("execvp() failed");
			exit(1);
		    }
		}
	
	    }
	    else if (childPid > 0) {
		// In the parent process, utilize the waidpid() command to wait for the child
		// process to finish executing
		pid = waitpid(childPid, &status, WNOHANG); // Return immediately if no child exited
	    	this->executeSuccess = WIFEXITED(status);	
	    }
	    else {
		// If this condition branch is reached the fork() process failed
		perror("fork() failed");
		exit(1);
	    }
	}
	virtual void print_command() {
	    cout << *command << separator << endl;
	} 
};

#endif //__Execute_HPP__
