#include <iostream>
#include "parser.hpp"
#include <string>
#include <vector>

using namespace std;

int main() {
   
    string userInput;
    vector<string> commands;
    
    while (1) { // Command shell should continue executing until exit is entered
	cout << "$ "; // Print command prompt
	getline(cin, userInput);		
	Parser* parsed = new Parser();
	commands = parsed->parse(userInput);
	parsed->execute(commands);
    }

    return 0;
}
