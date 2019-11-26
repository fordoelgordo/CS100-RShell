#include <iostream>
#include "executecommand.hpp"
//#include "executegroup.hpp"
//#include "execute.hpp"
#include "parser.hpp"
#include "exit.hpp"
#include "and.hpp"
#include <string>
#include <unistd.h> // To utilize execvp() command
#include <stdio.h>

using namespace std;

int main() {
   
    string userInput;
    
    while (1) { // Command shell should continue executing until exit is entered
	cout << "$ "; // Print command prompt
	getline(cin, userInput);		
	Parser* parsed = new Parser();
	parsed->parse(userInput);
	//ExecuteGroup* executable = parsed->parse(userInput);
	//executable->print_command();
	//FIXME
	//executable->execute();    
    }

    return 0;
}
