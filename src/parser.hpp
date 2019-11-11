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
	    if (finalCommands.size() == 1) {
		const char* cmd = finalCommands.at(0).c_str();
		ExecuteCommand* command = new Execute(cmd, sep);
		executable->add_command(command);
	    }
	    else {
		for (unsigned int i = 0; i < finalCommands.size() - 1; ++i) {
		    if ((finalCommands.at(i) != "&&" && finalCommands.at(i) != "||") && (finalCommands.at(i + 1) == "&&" || finalCommands.at(i + 1) == "||")) {
			const char* cmd2 = finalCommands.at(i).c_str();
			string sep2 = finalCommands.at(i + 1);
			ExecuteCommand* command2 = new Execute(cmd2, sep2);
			executable->add_command(command2);
			++i;
		    }
		    else {
			const char* cmd3 = finalCommands.at(i).c_str();
			ExecuteCommand* command3 = new Execute(cmd3, sep);
			executable->add_command(command3);
		    }
		}
    
		const char* cmd4 = finalCommands.at(finalCommands.size() - 1).c_str();
		ExecuteCommand* command4 = new Execute(cmd4, sep);
		executable->add_command(command4);
	    }

	    return executable;
	}				
};

#endif //__Parser_HPP__
