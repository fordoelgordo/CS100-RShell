#include "execute.hpp"

using namespace std;

Execute::Execute() : ExecuteCommand() {this->executeSuccess = false;}
Execute::Execute(char** enteredCommand, string sep) {
    this->command = enteredCommand;
    this->separator = sep;
}
void Execute::execute() {
    // Implement the command execution logic
    pid_t childPid;
    pid_t pid;
    int status;
    childPid = fork();
    
    if (childPid == 0) {
	// In the child process, execute commands using execvp()
	if(execvp(*this->command, this->command) < 0) {
	    perror("execvp() failed");
	    exit(1);
	}
    }
    else if (childPid > 0) {
	// In the parent process, utilize the waidpid() command to wait for the child
	// process to finish executing
	pid = waitpid(childPid, &status, 0); // Get status of child process
	if (pid == -1) {
	    perror("Error in child processing");
	    exit(1); // Exit with error status 1
	}
	if (status > 0) {
	    this->executeSuccess = false;
	}
	else if (status == 0) {
	    this->executeSuccess = true; // waitpid() returns 0 when child process returned 0
	}
	else if (status == 1) {
	    this->executeSuccess = false;
	}     			
    }
    else {
	// If this condition branch is reached the fork() process failed
	perror("fork() failed");
	exit(1);
    }
}
void Execute::print_command() {
    int i = 0;
    while (this->command[i] != '\0') {
	cout << this->command[i] << " ";
	++i;
    }
    cout << this->separator;
    cout << endl;
}
string Execute::get_command() {
    string cmd = *this->command;
    return cmd;
}
string Execute::get_command_full() {
    string full;
    int i = 0;
    while (this->command[i] != '\0') {
	full += this->command[i];
	++i;
    }

    return full;
}
string Execute::get_separator() {
    return this->separator;
}
bool Execute::get_success() {
    return this->executeSuccess;
}


