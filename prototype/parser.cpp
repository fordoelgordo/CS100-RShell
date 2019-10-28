#include <iostream>
#include <regex> // To use regex_replace to replace leading whitespace from string parser
#include <string>
#include <boost/tokenizer.hpp> // To use Tokenizer class from boost library for string parsing

using namespace std;

int main() {
    // Get user input from the command line
    string inputString;
    getline(cin, inputString);
          
    // Parse the string using the Tokenizer class, parsing on ;, &&, and ||
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(";", "&&||"); // Commands with ; are executed, commands with && and || are chained
    tokenizer tokens(inputString, sep);
    vector<string> commands;
    for (tokenizer::iterator iter = tokens.begin(); iter != tokens.end(); ++iter) {
	string parsed = *iter;
	parsed = regex_replace(parsed, regex("^ +"), "");
	commands.push_back(parsed);
    }

    // The parser separates && and ||, so we add those back together so that the full commands can
    // be properly executed
    vector<string> finalCommands;
    for (unsigned int i = 0; i < commands.size(); ++i) {
	if ((commands.at(i) == "&" && commands.at(i+1) == "&") || (commands.at(i) == "|" && commands.at(i + 1) == "|")) {
	    finalCommands.push_back(commands.at(i) + commands.at(i+1));
	    ++i;
	}
	else {
	    finalCommands.push_back(commands.at(i));
	}
    }

    for (unsigned int i = 0; i < finalCommands.size(); ++i) {
	cout << finalCommands.at(i) << endl;
    }

    return 0;
}

