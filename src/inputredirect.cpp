#include "inputredirect.hpp"

using namespace std;

// Constructors
InRedirect::InRedirect() : ExecuteCommand() {}
InRedirect::InRedirect(string command, string input) {
    Parser* parser = new Parser();
    this->command = parser->create_charstar(command);
    this->input = input;
}

// Execute() function
bool InRedirect::execute() {
    int file_desc = open(this->input.c_str(), O_RDONLY); // Open input file with read only access 
    
    pid_t childPid;
    pid_t pid;
    int status;
    childPid = fork();
    int saveinput = dup(0);
    
    if (childPid == 0) {
	if (file_desc < 0) {
	    perror("Error opening input file");
	    exit(1);
	}
	
	dup2(file_desc, 0); // replacing stdin with input file
	close(file_desc);
	if (execvp(*this->command, this->command) < 0) { //  Execute command
	    perror("execvp() failed");
	    exit(1);
	}
    }
    else if (childPid > 0) {
	// Wait for child to finish executing
	pid = waitpid(childPid, &status, 0);
	if (pid == -1) {
	    perror("Error in child processing");
	    exit(1);
	}
	if (status > 0) {
	    return false;
	}
	else if (status == 0) {
	    return true;
	}
	else if (status == 1) {
	    return false;
	}
    }
    else {
	// Condition check for fork() failure
	perror("fork() failed");
	exit(1);
    }
    
    close(saveinput); // Close fds
    dup2(saveinput, 0); // Copy original stdin back to stdin
    
    return true;
}

// Inherited functions, set them to do nothing for now
void InRedirect::print_command() {}
string InRedirect::get_command() {}
string InRedirect::get_command_full() {}
string InRedirect::get_separator() {}
