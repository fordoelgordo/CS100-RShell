#include <iostream>
#include <regex> // To use regex_replace to replace leading whitespace from string parser
#include <string>
#include <boost/tokenizer.hpp> // To use Tokenizer class from boost library for string parsing

using namespace std;

int main() {
    string inputString = "ls -a; echo hello && mkdir test || echo world; git status;";       
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(";", "&&||"); // Commands with ; are executed, commands with && and || are chained
    tokenizer tokens(inputString, sep);
    vector<string> commands;
    for (tokenizer::iterator iter = tokens.begin(); iter != tokens.end(); ++iter) {
	string parsed = *iter;
	parsed = regex_replace(parsed, regex("^ +"), "");
	commands.push_back(parsed);
    }

    vector<string> finalCommands;
    for (int i = 0; i < commands.size(); ++i) {
	if ((commands.at(i) == "&" && commands.at(i+1) == "&") || (commands.at(i) == "|" && commands.at(i + 1) == "|")) {
	    finalCommands.push_back(commands.at(i) + commands.at(i+1));
	    ++i;
	}
	else {
	    finalCommands.push_back(commands.at(i));
	}
    }

    for (int i = 0; i < finalCommands.size(); ++i) {
	cout << finalCommands.at(i) << endl;
    }

    return 0;
}

