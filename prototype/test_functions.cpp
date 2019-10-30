#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main() {

    char * command[] = {"ls", "-l", "-h", "-a", NULL};
    
    pid_t forkProcess;
    int stat_loc;
    
    forkProcess = fork();
    if (forkProcess == 0) {
	 execvp(command[0], command);
    }
    else {
	waitpid(forkProcess, &stat_loc, WUNTRACED);
    }

    return 0;
}
