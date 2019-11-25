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
    int j;
    if (commands.size() == 1) {
	finalCommands.push_back(commands.at(0));
    }
    else {
	for (unsigned int i = 0; i < commands.size() - 1; ++i) {
	    string join = commands.at(i);
	    if (commands.at(i).find("\"") != string::npos && commands.at(i).at(commands.at(i).size() - 2) != '"') {
		while (commands.at(i + 1).find("\"") == string::npos && i < commands.size()) {
		    join += commands.at(i + 1);
		    ++i;
		}
		++i;
		join += commands.at(i);
		finalCommands.push_back(join);
	    }
	    else {
		finalCommands.push_back(join);
	    }
	    j = i;
	}
    }
    if (j + 1 < commands.size()) {
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
    if (finalCommands.size() <= 2) {
	if (finalCommands.at(0).substr(0,4) == "test" || finalCommands.at(0).substr(0,1) == "[") { // Create TestExecute object if user entered a "test" command
	    ExecuteCommand* test = new TestExecute(create_stringvec(finalCommands.at(0)), sep);
	    executable->add_command(test);
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
		    if (finalCommands.at(i).substr(0,4) == "test" || finalCommands.at(i).substr(0,1) == "[") {
			ExecuteCommand* test = new TestExecute(create_stringvec(finalCommands.at(i)), finalCommands.at(i + 1));
			executable->add_command(test);
		    }
		    else {
			ExecuteCommand* command = new Execute(create_charstar(finalCommands.at(i)), finalCommands.at(i + 1));
			executable->add_command(command);
		    }
		    ++i;
		}
	    }
	}
	int pos = finalCommands.size() - 1;
	if (finalCommands.at(pos) != "&&" && finalCommands.at(pos) != "||" && finalCommands.at(pos) != ";") {
	    if (finalCommands.at(pos).substr(0,4) == "test" || finalCommands.at(pos).substr(0,1) == "[") {
		ExecuteCommand* test2 = new TestExecute(create_stringvec(finalCommands.at(pos)), sep);
		executable->add_command(test2);
	    }
	    else {
		ExecuteCommand* command2 = new Execute(create_charstar(finalCommands.at(pos)), sep);
		executable->add_command(command2);
	    }
	}
    }
       
    return executable;
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
vector<string> Parser::create_stringvec(const string & input) {
    char* cmd;
    char* token;
    vector<string> arguments;
    cmd = (char*)input.c_str();
    token = strtok(cmd, " ");
    while (token != NULL) {
	arguments.push_back(std::string(token));
	token = strtok(NULL, " ");
    }
   
    vector<string> v;
    if (arguments.at(0) == "test") {
	v = arguments;
    }
    else {
	for (unsigned int i = 0; i < arguments.size(); ++i) {
	    if (arguments.at(i).find("[") != string::npos) {
		v.push_back("[");
		v.push_back(arguments.at(i).substr(arguments.at(i).find("[") + 1,arguments.at(i).size()));
	    }
	    else if (arguments.at(i).find("]") != string::npos) {
		v.push_back(arguments.at(i).substr(0, arguments.at(i).size() - 1));
	    }
	    else {
		v.push_back(arguments.at(i));
	    }
	}
	v.push_back("]");
    }

    return v;
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
