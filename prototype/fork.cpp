#include <iostream>
#include <unistd.h> // To use fork() function

using namespace std;

int main() {

    pid_t test_fork = fork();

    if (test_fork == 0) {
	cout << "---------------------" << endl;
	cout << "Child process fork" << endl;
	cout << "Process ID: " << getpid() << endl;
	cout << "Child ID: " << test_fork << endl;
     }
    else {
	cout << "Parent process fork" << endl;
	cout << "Process ID: " << getpid() << endl;
	cout << "Child ID: " << test_fork << endl;
    }

    return 0;
}
