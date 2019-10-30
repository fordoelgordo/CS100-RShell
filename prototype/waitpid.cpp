#include <iostream>
#include <unistd.h> // To use the fork() function
#include <sys/wait.h> // To use the waitpid() function

using namespace std;

int main() {
    pid_t forkProcess;
    pid_t waitResult;
    int statLoc;

    forkProcess = fork();
    if (forkProcess == 0) {
	cout << "Child process" << endl;
	cout << "Process ID: " << getpid() << endl;
	cout << "Child ID: " << forkProcess << endl;
    }
    else {
	waitResult = waitpid(forkProcess, &statLoc, WUNTRACED); // The parent block will not get executed until the child block finishes executing
	cout << "---------------" << endl;
	cout << "Parent process" << endl;
	cout << "Process ID: " << getpid() << endl;
	cout << "Child ID: " << forkProcess << endl;
    }

    return 0;
} 
