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
		parsed = regex_replace(parsed, regex("^ +"), ""); // Remove leading whitespace from parsed token
		parsed = regex_replace(parsed, regex(" +$"), ""); // Remove trailing whitespace from parsed token
		commands.push_back(parsed);
	    }

	    // Create a vector of entered commands, which we will built Executable objects off of
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
   
	    // Create Executable objects 
	    ExecuteGroup* executable = new ExecuteGroup();
	    string sep = ";";
	    char* cmd;
	    vector<char*> args;
	    char* temp;
	    if (finalCommands.size() == 1) {
		cmd = (char*)finalCommands.at(0).c_str();
		temp = strtok(cmd, " "); // Split command by spaces
		while (temp != NULL) {
		    if (temp != '\0') {
			args.push_back(temp);
		    }
		    temp = strtok(NULL, " ");
		}
		char** arguments = new char*[args.size() + 1];
		for (int i = 0; i < args.size(); ++i) {
		    arguments[i] = args.at(i);
		}
		arguments[args.size()] = NULL; // Ensure arguments list is NULL terminated	
		ExecuteCommand* command = new Execute(arguments, sep);
		executable->add_command(command);
	    }
	    else {
		for (unsigned int i = 0; i < finalCommands.size() - 1; ++i) {
		    cmd = (char*)finalCommands.at(i).c_str();
		    temp = strtok(cmd, " ");
		    while (temp != NULL) {
			if (temp != '\0') {
			    args.push_back(temp);
			}
			temp = strtok(NULL, " ");
		    }
		    char** arguments2 = new char*[args.size() + 1];
		    for (int i = 0; i < args.size(); ++i) {
			arguments2[i] = args.at(i);
		    }
		    arguments2[args.size()] = NULL;   
		    if ((finalCommands.at(i) != "&&" && finalCommands.at(i) != "||") && (finalCommands.at(i + 1) == "&&" || finalCommands.at(i + 1) == "||")) {
			ExecuteCommand* command2 = new Execute(arguments2, finalCommands.at(i + 1));
			executable->add_command(command2);
			++i;
		    }
		    else {
			ExecuteCommand* command3 = new Execute(arguments2, sep);
			executable->add_command(command3);
		    }
		    args.clear();
		}
	    }

	    return executable;
	}				
};

#endif //__Parser_HPP__
