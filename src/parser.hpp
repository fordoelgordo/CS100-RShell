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
	    boost::char_separator<char> delim("", "&|;"); // Parse on & | and ; but keep these tokens in the output
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
	    
	    // Correctly account for connectors between " "
	    vector<string> finalCommands;
	    for (unsigned int i = 0; i < commands.size() - 1; ++i) {
		string join = commands.at(i);
		if (commands.at(i).find("\"") != string::npos) {
		    while (commands.at(i + 1).find("\"") == string::npos) {
			join += commands.at(i + 1);
			++i;
		    }
		    ++i;
		    join += commands.at(i);
		}
		finalCommands.push_back(join);
	    }
	    if (commands.at(commands.size() - 1).find("\"") == string::npos) {
		finalCommands.push_back(commands.at(commands.size() - 1));
	    }
	    
	    // If there are connectors at the beginning of the command sequence, remove them
	    while (finalCommands.at(0) == "&" || finalCommands.at(0) == "|" || finalCommands.at(0) == ";") {
		finalCommands.erase(finalCommands.begin() + 0);
	    }

	    // Link separated connectors together
	    for (unsigned int i = 0; i < finalCommands.size() - 1; ++i) {
		if ((finalCommands.at(i) == "&" && finalCommands.at(i + 1) == "&") || (finalCommands.at(i) == "|" && finalCommands.at(i + 1) == "|") || (finalCommands.at(i) == ";" && finalCommands.at(i + 1) == ";")) {
		    finalCommands.at(i) += finalCommands.at(i + 1);
		    ++i;
		}
	    }
	    
	    // Parse out comments
	    for (unsigned int i = 0; i < finalCommands.size(); ++i) {
		if (finalCommands.at(i).find("#") != string::npos) {
		    finalCommands.at(i).erase(finalCommands.at(i).find("#"), finalCommands.at(i).size());
		}
	    }

	    // Parse out " " since we've handled correctly so that it doesn't print to console
	    for (unsigned int i = 0; i < finalCommands.size(); ++i) {
		finalCommands.at(i).erase(remove(finalCommands.at(i).begin(), finalCommands.at(i).end(), '\"'), finalCommands.at(i).end());	
	    }  
	    
	    // Create Executable objects
	    ExecuteGroup* executable = new ExecuteGroup();

	    if (finalCommands.size() == 1) {
		char** arguments = create_charstar(finalCommands.at(0));
		ExecuteCommand* command = new Execute(arguments, ";");
		executable->add_command(command);
	    }
	    else {
		for (unsigned int i = 0; i < finalCommands.size() - 1; ++i) {
		    cout << "Creating execute with: " << finalCommands.at(i) << endl;
		    char** arguments2 = create_charstar(finalCommands.at(i));
		    if (finalCommands.at(i) != "&&" && finalCommands.at(i) != "&" && finalCommands.at(i) != "||" && finalCommands.at(i) != "|" && finalCommands.at(i) != ";") {
			if (finalCommands.at(i + 1) == "&&" || finalCommands.at(i + 1) == "||" || finalCommands.at(i + 1) == ";") {
			    ExecuteCommand* command2 = new Execute(arguments2, finalCommands.at(i + 1));
			    executable->add_command(command2);
			    ++i;
			}
			else {
			    ExecuteCommand* command3 = new Execute(arguments2, ";");
			    executable->add_command(command3);
			}
		    }
		}
		int pos = finalCommands.size() - 1;
		if (finalCommands.at(pos) != "&&" && finalCommands.at(pos) != "||" && finalCommands.at(pos) != ";" && finalCommands.at(pos) != "&" && finalCommands.at(pos) != "|") {
		    char** arguments3 = create_charstar(finalCommands.at(pos));
		    ExecuteCommand* command4 = new Execute(arguments3, ";");
		    executable->add_command(command4);
		}
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
