#ifndef __Parser_HPP__
#define __Parser_HPP__

#include "executecommand.hpp"
#include "executegroup.hpp"
#include "execute.hpp"
#include <string>
#include <regex>
#include <boost/tokenizer.hpp>

using namespace std;

class Parser {
    public: 
	Parser() {};
	ExecuteGroup* parse(string userInput) {
	    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	    boost::char_separator<char> delim(";", "&&||");
	    tokenizer tokens(userInput, delim);
	    vector<string> commands;
	   
	    // Parse userInput into tokens using the ;, &&, || separators
	    for (tokenizer::iterator iter = tokens.begin(); iter != tokens.end(); ++iter) {
		string parsed = *iter;
		if (parsed.find("\"") == string::npos) {
		    parsed = regex_replace(parsed, regex("^ +"), "");
		    parsed = regex_replace(parsed, regex(" +$"), ""); // Remove trailing whitespace from parsed token
		}
		commands.push_back(parsed);
	    }

	    // Create a vector of entered commands, which we will build Executable objects off of
	    vector<string> finalCommands;
	    for (unsigned int i = 0; i < commands.size() - 1; ++i) {
		if ((commands.at(i) == "&" && commands.at(i + 1) == "&") || (commands.at(i) == "|" && commands.at(i + 1) == "|")) {
		    finalCommands.push_back(commands.at(i) + commands.at(i + 1));
		    ++i;
		}
		else {
		    finalCommands.push_back(commands.at(i));
		}
	    }
	    finalCommands.push_back(commands.at(commands.size() - 1));
	    
	    // Parse out comments
	    for (unsigned int i = 0; i < finalCommands.size(); ++i) {
		if (finalCommands.at(i).find("#") != string::npos) {
		    finalCommands.at(i).erase(finalCommands.at(i).find("#"), finalCommands.at(i).size());
		}
	    }
	    
	    // Correctly account for " " in commands
	    for (unsigned int i = 0; i < finalCommands.size(); ++i) {
		if (finalCommands.at(i).find("\"") != string::npos && i < finalCommands.size() - 2) {
		    finalCommands.at(i) = regex_replace(finalCommands.at(i), regex("^ +"), "");
		    finalCommands.at(i) = finalCommands.at(i) + finalCommands.at(i + 1) + finalCommands.at(i + 2);
		    finalCommands.erase(finalCommands.begin() + i + 1); // Erase portions of vector that have been combined
		    finalCommands.erase(finalCommands.begin() + i + 1); // Erase portions of vector that have been combined
		}
	    }
	
	    // Create Executable objects 
	    ExecuteGroup* executable = new ExecuteGroup();
	    string sep = ";";
	    if (finalCommands.size() == 1) {
		char** arguments = create_charstar(finalCommands.at(0));
		ExecuteCommand* command = new Execute(arguments, sep);
		executable->add_command(command);
	    }
	    else {
		for (unsigned int i = 0; i < finalCommands.size() - 1; ++i) {
		    char** arguments2 = create_charstar(finalCommands.at(i));
		    if ((finalCommands.at(i) != "&&" && finalCommands.at(i) != "||") && (finalCommands.at(i + 1) == "&&" || finalCommands.at(i + 1) == "||")) {
			ExecuteCommand* command2 = new Execute(arguments2, finalCommands.at(i + 1));
			executable->add_command(command2);
			++i;
		    }
		    else {
			ExecuteCommand* command3 = new Execute(arguments2, sep);
			executable->add_command(command3);
		    }
		}
		char** arguments3 = create_charstar(finalCommands.at(finalCommands.size() - 1));
		ExecuteCommand* command4 = new Execute(arguments3, sep);
		executable->add_command(command4);
	    }

	    return executable;
	}
    
    private:
	// Implement helper functions to main parsing function
	char** create_charstar(string input) {
	    char* cmd;
	    vector<char*> args;
	    char* temp;
	    cmd = (char*)input.c_str();
	    temp = strtok(cmd, " ");
	    while (temp != NULL) {
		if (temp != '\0') {
		    args.push_back(temp);
		}
		temp = strtok(NULL, " ");
	    }
	    char** arguments = new char*[args.size() + 1];
	    for (unsigned int i = 0; i < args.size(); ++i) {
		arguments[i] = args.at(i);
	    }
	    arguments[args.size()] = NULL;

	    return arguments;
	}
				
};

#endif //__Parser_HPP__
