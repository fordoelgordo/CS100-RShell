#include "Pipe.hpp"

using namespace std;


Pipe::Pipe() {}

Pipe::Pipe(string left, ExecuteCommand *r) {
	this->left = left;
	this->right = r;
}

bool Pipe::execute() {
	//work in progress, struggling to implement pipe with fork();
	int pipeW[2]; //Write Pipe
	int save_in;//dup[0]
	int save_out;//dup[1]

	pipe(pipeW);

	pid_t childPid;
	pid_t pid;
	int status;

	if(childPid == 0) {
		if(execvp(*this->command, this->command) < 0) {
			perror("execvp() failed");
			exit(1);
		}
	}
	else if (childPid > 0) {
	
		pid = waitpid(childPid, &status, 0);
		if (pid == -1) {
			perror("Error in child processing");
			exit(1);
		}
		if (status > 0) {
			return false;
		}
		else if(status == 0) {
			return true;
		}
		else if (status == 1) {
			return false;
		}
	}
	else {
		perror("fork() failed");
		exit(1);
	}
}
