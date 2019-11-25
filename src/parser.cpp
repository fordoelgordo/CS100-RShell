#include "parser.hpp"

using namespace std;

class ExecuteCommand;

Parser::Parser() {}
ExecuteGroup* Parser:: parse(string userInput) {
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> delim("", "&|;"); // Parse on & | and ; but keep these tokens in the output
    tokenizer tokens(userInput, delim);
    vector<string> commands;
   
    // Parse userInput into tokens using the ;, &&, || separators
    for (tokenizer::iterator iter = tokens.begin(); iter != tokens.end(); ++iter) {
	string parsed = *iter;
	/* FIXME POTENTIALLY	
	if (parsed.find("\"") == string::npos) {
	    parsed = regex_replace(parsed, regex("^ +"), "");
	    parsed = regex_replace(parsed, regex(" +$"), ""); // Remove trailing whitespace from parsed token
	}
	*/
	commands.push_back(parsed);
    }
    
    // Correctly account for connectors between " "
    vector<string> finalCommands;
    if (commands.size() == 1) {
	finalCommands.push_back(commands.at(0));
    }
    else {
	for (unsigned int i = 0; i < commands.size() - 1; ++i) {
	    string join = commands.at(i);
	    if (commands.at(i).find("\"") != string::npos && commands.at(i).substr(commands.at(i).find("\"") + 1, commands.at(i).size()).find("\"") == string::npos) {
		while (commands.at(i + 1).find("\"") == string::npos) {
		    join += commands.at(i + 1);
		    ++i;
		}
		++i;
		if (commands.at(i).find("\"") != string::npos) {
		    join += commands.at(i);
		}
	    }
	    finalCommands.push_back(join);
	}	
    }
    if (commands.at(commands.size() - 1).find("\"") == string::npos) {
	finalCommands.push_back(commands.at(commands.size() - 1));
    }
    
    // Remove leading and trailing white space from commands that are not between " "
    for (unsigned int i = 0; i < finalCommands.size(); ++i) {
	if (finalCommands.at(i).find("\"") == string::npos) {
	    finalCommands.at(i) = regex_replace(finalCommands.at(i), regex("^ +"), "");
	    finalCommands.at(i) = regex_replace(finalCommands.at(i), regex(" +$"), "");
	}
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
    int index = -1;
    bool found = false;
    for (unsigned int i = 0; i < finalCommands.size() && !found; ++i) {
	if (finalCommands.at(i).find("#") != string::npos) {
	    finalCommands.at(i).erase(finalCommands.at(i).find("#"), finalCommands.at(i).size());
	    found  = true;
	    index = i;
	}
    }
    if (index != -1) {
	finalCommands.erase(finalCommands.begin() + index + 1, finalCommands.end());
    }    

    // Parse out " " since we've handled correctly so that it doesn't print to console
    for (unsigned int i = 0; i < finalCommands.size(); ++i) {
	finalCommands.at(i).erase(remove(finalCommands.at(i).begin(), finalCommands.at(i).end(), '\"'), finalCommands.at(i).end());	
    }   

    // Remove all instance of a single "&" and "|"
    finalCommands.erase(std::remove(finalCommands.begin(), finalCommands.end(), "&"), finalCommands.end());
    finalCommands.erase(std::remove(finalCommands.begin(), finalCommands.end(), "|"), finalCommands.end());
 
    // Create Executable objects
    ExecuteGroup* executable = new ExecuteGroup();
    string sep = ";";
    if(check_valid_par(finalCommands)) {
    	if (finalCommands.size() <= 2) {
		if(finalCommands.at(0).substr(0, 1) == "(") {
			parse_par(finalCommands);//Parse parenthesis from finalCommands, and create new Execute object
			ExecuteCommand *command = new Execute(create_charstar(finalCommands.at(0)), sep);
			executable->add_command(command);
		}
		else {
			char** arguments = create_charstar(finalCommands.at(0));
			ExecuteCommand* command = new Execute(arguments, sep);
			executable->add_command(command);
		}	
   	 }

    	else {
		for (unsigned int i = 0; i < finalCommands.size() - 1; ++i) {
	    	if (finalCommands.at(i) != "&&" && finalCommands.at(i) != "||" && finalCommands.at(i) != ";") {
			if (finalCommands.at(i + 1) == "&&" || finalCommands.at(i + 1) == "||" || finalCommands.at(i + 1) == ";") {
				parse_par(finalCommands);
				ExecuteCommand *command = new Execute(create_charstar(finalCommands.at(i)), finalCommands.at(i + 1));
				executable->add_command(command);
			}
		
			else {	 
				parse_par(finalCommands);	
				ExecuteCommand *command = new Execute(create_charstar(finalCommands.at(i)), finalCommands.at(i + 1));
				executable->add_command(command);
			}
				++i;
			}
		}

		int pos = finalCommands.size() - 1;
		if (finalCommands.at(pos) != "&&" && finalCommands.at(pos) != "||" && finalCommands.at(pos) != ";") {
			if(finalCommands.at(pos).substr(0,1) == "(") {
				ExecuteCommand *command = new Execute(create_charstar(finalCommands.at(pos)), sep);
				executable->add_command(command);
			}
			else {
				ExecuteCommand *command = new Execute(create_charstar(finalCommands.at(pos)), sep);
				executable->add_command(command);
			}
			}
		}
    		return executable;
	}
	
	//If inbalance parenthesis, close the program
	else {
		cout << "Invalid Parenthesis!" << endl;
		cout << "Exiting..." << endl;
		exit(1);
	}
}

//Checks if input is valid by checking if input have proper left and right parenthesis
bool Parser::check_valid_par(vector<string> &input) {

	int left_par = 0;
	int right_par = 0;

	for(unsigned int i = 0; i < input.size(); ++i) {
		for(unsigned int j = 0; j < input.at(i).size(); ++j) {
			if(input.at(i).at(j) == '(') {
				++left_par;
			}
			if(input.at(i).at(j) == ')') {
				++right_par;
			}
		}
	}
	if(left_par != right_par) {
		return false;
	}
	return true;
}

//Parse parenthesis from FinalCommand
void Parser::parse_par(vector<string> &final_input) {

	for(unsigned int i = 0 ; i < final_input.size(); ++i) {
		for(unsigned int j = 0; j < final_input.at(i).size(); ++j) {
  	               if(final_input.at(i).at(j) == '(' || final_input.at(i).at(j) == ')') {
				if(final_input.at(i).at(j) == '(') {
                   	    		final_input.at(i).erase(j, 1);
				}
			}
			//Second loop to fix double parenthesis bug. Ex. ((echo hello world) ==> (echo hello world
          		for(unsigned int j = 0; j < final_input.at(i).size(); ++j) {
                  		if(final_input.at(i).at(j) == '(' || final_input.at(i).at(j) == ')') {
                           		  final_input.at(i).erase(j, 1);
                        	}
       	                }
		}          	
	}
}

// Implement helper functions to main parsing function
char** Parser::create_charstar(const string & input) {
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

void Parser::print_charstar(char** input) {
    int i = 0;
    while (input[i] != '\0') {
	cout << input[i] << " ";
	++i;
    }
    cout << endl;
}

void Parser::print_vector(const vector<string> & vec) {
    for (unsigned int i = 0; i < vec.size(); ++i) {
	cout << vec.at(i) << endl;
    }
}
