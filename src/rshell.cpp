#include <iostream>
#include "executecommand.hpp"
#include "executegroup.hpp"
#include "execute.hpp"
#include "parser.hpp"
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
	ExecuteGroup* executable = parsed->parse(userInput);
	executable->execute();    
    }

    return 0;
}
