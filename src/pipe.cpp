#include "pipe.hpp"

using namespace std;

// Constructors
Pipe::Pipe() : ExecuteCommand() {}
Pipe::Pipe(vector<string> commands) {
    this->commands = commands;
}

// Execute() function
bool Pipe::execute() {
    string left;
    string right;
    queue<string> connectors;

    for (unsigned int i = 0; i < this->commands.size(); ++i) {
	if (this->commands.at(i) != "|") {
	    if (connectors.empty()) {
		left = this->commands.at(i);
	    }
	    else {
		right = this->commands.at(i);
		if (i == this->commands.size() - 1) {
		    executePipes(left, right);
		}
	    }
	}
	else {
	    connectors.push(this->commands.at(i));
	}
    }

    return true;
}

// executePipes() helper function that does the actual processing of the input between pipes
bool Pipe::executePipes(string left, string right) {
    Parser* parser = new Parser();
    char** leftCommand = parser->create_charstar(left);

    int pipefd[2]; // Create integer array of size 2
    pipe(pipefd); // Make a pipe

    // Create fork() process
    pid_t childPid;
    pid_t pid;
    int status;
    childPid = fork();
    
    if (childPid == 0) {
	// In the child process
	close(pipefd[0]); // Close pipe input
	int saveoutput = dup(1);
	int file_desc = dup2(pipefd[1], 1); // Copy process in pipefd[1] to fileout
	if (file_desc < 0) {
	    perror("File does not exist");
	    exit(1);
	}
	if (file_desc == -1) {
	    perror("Error in dup2() processing");
	    exit(1);
	}
	if (execvp(*leftCommand, leftCommand) < 0) {
	    perror("execvp() failed");
	    exit(1);
	}
	close(pipefd[1]); // Close pipe output
	dup2(saveoutput, 1);
    }
    else if (childPid > 0) {
	pid = waitpid(childPid, &status, 0);
	close(pipefd[1]);
	int saveinput = dup(0);
	dup2(pipefd[0], 0);

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

	char** rightCommand = parser->create_charstar(right);
	if (execvp(*rightCommand, rightCommand) < 0) {
	    perror("execvp() failed");
	    exit(1);
	}
	close(pipefd[0]);
	dup2(saveinput, 0);
    }
    else {
	// fork() failed
	perror("fork() failed");
	exit(1);
    }

    return true;
}

// Inherited functions, set them to do nothing for now
void Pipe::print_command() {}
string Pipe::get_command() {}
string Pipe::get_command_full() {}
string Pipe::get_separator() {}







