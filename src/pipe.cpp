#include "pipe.hpp"

using namespace std;

// Constructors
Pipe::Pipe() : ExecuteCommand() {}
Pipe::Pipe(vector<string> commands) {
    this->commands = commands;
}

// Execute() function
bool Pipe::execute() {
    // Save current stdin and stdout into new file descriptors
    int tempin = dup(0);
    int tempout = dup(1);

    // Check for input redirection
    int file_in;
    for (unsigned int i = 0; i < this->commands.size(); ++i) {
	if (this->commands.at(i) == "<") {
	    file_in = open(this->commands.at(i + 1).c_str(), O_RDONLY); // Set input file
	    break;
	}
	else {
	    file_in = dup(tempin);
	}
    }    

    // Remove <, >, >> and | from the command sequence
    vector<string> commands2;
    for (unsigned int i = 0; i < this->commands.size(); ++i) {
	if (this->commands.at(i) == ">" || this->commands.at(i) == "<" || this->commands.at(i) == ">>") {
	    // Do not process the command after i
	    ++i;
	}
	else if (this->commands.at(i) != "|") {
	    commands2.push_back(this->commands.at(i));
	}
    }

    // Iterate through commands
    int file_out;
    Parser* parser = new Parser();
    char** pipeCommand;
    pid_t childPid;
    pid_t pid;
    int status;
    for (unsigned int i = 0; i < commands2.size(); ++i) {
	dup2(file_in, 0);
	close(file_in);
	if (i == commands2.size() - 1) {
	    if (this->commands.at(this->commands.size() - 2) == ">" || this->commands.at(this->commands.size() - 2) == ">>") {
		file_out = open(this->commands.at(this->commands.size() - 1).c_str(), O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP);
	    }
	    else {
		file_out = dup(tempout);
	    }
	}
	else { // Not at last command
	    // Create pipe
	    int fdpipe[2];
	    pipe(fdpipe);
	    file_out = fdpipe[1];
	    file_in = fdpipe[0];
	}

	// Redirect output
	dup2(file_out, 1);
	close(file_out);

	// Create child process
	pipeCommand = parser->create_charstar(commands2.at(i));
	childPid = fork();
	if (childPid == 0) {
	    // In child process
	    if (execvp(*pipeCommand, pipeCommand) < 0) {
		perror("execvp() failed");
		exit(1);
	    }
	}
    } // End for
    
    // Restore in/out defaults
    dup2(tempin, 0);
    dup2(tempout, 1);
    close(tempin);
    close(tempout);
    
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
    
    return true;	
}


// Inherited functions, set them to do nothing for now
void Pipe::print_command() {}
string Pipe::get_command() {}
string Pipe::get_command_full() {}
string Pipe::get_separator() {}







