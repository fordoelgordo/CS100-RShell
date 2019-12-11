#include "outputredirect.hpp"

using namespace std;

// Constructors
OutRedirect::OutRedirect() : ExecuteCommand() {}
OutRedirect::OutRedirect(string input, string output) {
    Parser* parser = new Parser();
    this->command = parser->create_charstar(input);
    this->output = output;
}

// Execute() function
bool OutRedirect::execute() {
    // Get file descriptor for output file
    int file_desc = open(this->output.c_str(), O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP);
    /* Note the definition of the following flags in linux's open() command:
       O_CREAT: if pathname does not exist, it is created as a regular file
       O_WRONLY: set access mode to write only so that file can only be written to
       O_TRUNC: if the file already exists and is a regular file (set by O_CREAT) and access mode allows writing, truncate to length 0
       S_IRUSR: the user has read permission
       S_IRGRP: the group has read permission
       S_IWGRP: the group has write permission
       S_IWUSR: the user has write permission
    */
    pid_t childPid;
    pid_t pid;
    int status;
    childPid = fork();
    int saveoutput = dup(1); // Copying current stdout to saveoutput

    if (childPid == 0) {
	// In child process, replace stdout with file_desc and execute command	
	if (file_desc < 0) {
	    perror("Error in opening output file");
	    exit(1);
	}
	dup2(file_desc, 1); // replacing stdout with output file
	close(file_desc); // closing file descriptor
	if (execvp(*this->command, this->command) < 0) { // Executing command
	    perror("execvp() failed");
	    exit(1);
	}
    }
    else if (childPid > 0) {
	// In parent process, wait for child to finish processing, and error check
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
	// If this condition branch is reached then the fork() process failed
	perror("fork() failed");
	exit(1);
    }
    close(saveoutput); // Close fds
    dup2(saveoutput, 1); // Copy original stdout back to stdout
    
    return true;
}

// Inherited functions, set them to doing nothing for now
void OutRedirect::print_command() {}
string OutRedirect::get_command() {}
string OutRedirect::get_command_full() {}
string OutRedirect::get_separator() {}

