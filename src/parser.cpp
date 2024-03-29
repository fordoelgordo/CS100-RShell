#include "parser.hpp"
#include "factory.hpp"

using  namespace std;

Parser::Parser() {}
vector<string> Parser:: parse(string userInput) {
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> delim("", "&|;"); // Parse on & | and ; but keep these tokens in the output
    tokenizer tokens(userInput, delim);
    vector<string> commands;
   
    // Parse userInput into tokens using the ;, &&, || separators
    for (tokenizer::iterator iter = tokens.begin(); iter != tokens.end(); ++iter) {
	string parsed = *iter;
	/*  POTENTIALLY	
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
    
    // Erase the "single" connectors that follow the double connectors
    for (unsigned int i = 0; i < finalCommands.size(); ++i) {
	if (finalCommands.at(i) == "||" && finalCommands.at(i + 1) == "|") {
	    finalCommands.erase(finalCommands.begin() + i + 1, finalCommands.begin() + i + 2);
	}
	if (finalCommands.at(i) == "&&" && finalCommands.at(i + 1) == "&") {
	    finalCommands.erase(finalCommands.begin() + i + 1, finalCommands.begin() + i + 2);
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
    // Implemented fix above: finalCommands.erase(std::remove(finalCommands.begin(), finalCommands.end(), "&"), finalCommands.end());
    // Implemneted fix above: finalCommands.erase(std::remove(finalCommands.begin(), finalCommands.end(), "|"), finalCommands.end());
 
    // Parse out parentheses, < , >, >> place on separate lines
    vector<string> finalCommands2;
    stack<string> temp;
    for (unsigned int i = 0; i < finalCommands.size(); ++i) {
	int j = 0;
	while (j >= 0 && j < finalCommands.at(i).size()) {   
	    if (finalCommands.at(i).at(j) == '(' || finalCommands.at(i).at(j) == ')') {
		if (finalCommands.at(i).at(j) == '(') {
		    finalCommands2.push_back(string(1, finalCommands.at(i).at(j)));
		}
		if (finalCommands.at(i).at(j) == ')') {
		    temp.push(string(1, finalCommands.at(i).at(j)));
		}
		finalCommands.at(i).erase(j, 1);
		j = 0;
	    }
	    else if (finalCommands.at(i).find("(") == string::npos && finalCommands.at(i).find(")") == string::npos) {
		finalCommands2.push_back(finalCommands.at(i));
		j = finalCommands.at(i).size();
	    }
	    else {
		++j;
	    }
	}
	while (!temp.empty()) {
	    finalCommands2.push_back(temp.top());
	    temp.pop();
	}
    }

    typedef boost::tokenizer<boost::char_separator<char> > redirection;
    boost::char_separator<char> delim2("", "<>"); // Parse on & | and ; but keep these tokens in the output
    vector<string> finalCommands3;
    for (int i = 0; i < finalCommands2.size(); ++i) {
	redirection tokens(finalCommands2.at(i), delim2);
	for (redirection::iterator iter = tokens.begin(); iter != tokens.end(); ++iter) {
	    string redirect = *iter;
	    finalCommands3.push_back(redirect);
	}
    }
    
    // Link separated >> together
    for (unsigned int i = 0; i < finalCommands3.size() - 1; ++i) {
	if (finalCommands3.at(i) == ">" && finalCommands3.at(i + 1) == ">") {
	    finalCommands3.at(i) += finalCommands3.at(i + 1);
	    ++i;
	}
    }

    // Erase the "single" connectors that follow the double connectors
    for (unsigned int i = 0; i < finalCommands3.size(); ++i) {
	if (finalCommands3.at(i) == ">>" && finalCommands3.at(i + 1) == ">") {
	    finalCommands3.erase(finalCommands3.begin() + i + 1, finalCommands3.begin() + i + 2);
	}
    }
    
    // Remove leading and trailing white space introduced from second parsing
    for (unsigned int i = 0; i < finalCommands3.size(); ++i) {
	if (finalCommands3.at(i).find("\"") == string::npos) {
	    finalCommands3.at(i) = regex_replace(finalCommands3.at(i), regex("^ +"), "");
	    finalCommands3.at(i) = regex_replace(finalCommands3.at(i), regex(" +$"), "");
	}
    }
    
    // Check for matching parentheses
    int leftParen = 0;
    int rightParen = 0;
    for (unsigned int i = 0; i < finalCommands3.size(); ++i) {
	if (finalCommands3.at(i) == ")") {
	    ++rightParen;
	}
	if (finalCommands3.at(i) == "(") {
	    ++leftParen;
	}
    }
    
    if (leftParen != rightParen) {
	cout << "Error: non-matching set of parentheses entered" << endl;
	exit(1);
    }
    
    return finalCommands3;
}
bool Parser::execute(vector<string> userInput) {
    // Evaluate connectors 
    int leftParen = 0;
    int rightParen = 0;
    int connCount = 0;
    int redirectCount = 0;
    int pipeCount = 0;
    bool success = true;
    int redirectExecute = 0;
    vector<string> evalLeft;
    vector<string> evalRight;
    vector<string> parentheses;
    queue<string> connectors;

    for (unsigned int i = 0; i < userInput.size(); ++i) {
	if (userInput.at(i) == "(") {
	    ++leftParen;
	}
	else if (userInput.at(i) == ")") {
	    ++rightParen;
	}
	else if (userInput.at(i) == "&&" || userInput.at(i) == "||" || userInput.at(i) == ";") {
	    ++connCount;
	}
	else if (userInput.at(i) == "<" || userInput.at(i) == ">" || userInput.at(i) == ">>") {
	    ++redirectCount;
	}
	else if (userInput.at(i) == "|") {
	    ++pipeCount;
	}
    }
 
    // No pipes in commmand string, redirect commands found
    if (redirectCount > 0 && pipeCount == 0) {
	for (unsigned int i = 0; i < userInput.size(); ++i) {
	    if (userInput.at(i) == ">" || userInput.at(i) == "<" || userInput.at(i) == ">>") {
		connectors.push(userInput.at(i));
	    }
	    else if (connectors.empty()) {
		evalLeft.push_back(userInput.at(i));
	    }
	    else {
		evalRight.push_back(userInput.at(i));
	    }
	}
	if (connectors.front() == ">" || connectors.front() == ">>") {
	    OutRedirect* out = new OutRedirect(evalLeft.at(0), evalRight.at(0));
	    return out->execute();
	}
	else if (connectors.front() == "<") {
	    InRedirect* in = new InRedirect(evalLeft.at(0), evalRight.at(0));
	    return in->execute();
	}
    }
    
    // Pipes encountered in the commands
    if (pipeCount > 0) {
	Pipe* p = new Pipe(userInput);
	return p->execute();
    }

    // No connectors present in command sequence
    if (connCount == 0) {
	Factory* factory = new Factory();
	ExecuteCommand* executable = nullptr;
	for (unsigned int i = 0; i < userInput.size(); ++i) {
	    if (userInput.at(i) != ")" && userInput.at(i) != "(") {
		executable = factory->create_command(userInput.at(i), ";");
		return executable->execute();
	    }
	}
    }
     
    // Handle a single connector 
    if (connCount == 1) {
	// Loop through the commands, add connectors to connector queue
	for (unsigned int i = 0; i < userInput.size(); ++i) {
	    if (userInput.at(i) == "(" || userInput.at(i) == ")") {
		// Do nothing, precedence does not matter in this situation
	    }
	    else if (userInput.at(i) != "&&" && userInput.at(i) != "||" && userInput.at(i) != ";") {
		if (connectors.empty()) {
		    evalLeft.push_back(userInput.at(i));
		}
		else {
		    evalRight.push_back(userInput.at(i));
		}
	    }
	    else {
		// Add connector to the queue
		connectors.push(userInput.at(i));
	    }
	}
	Factory* factory = new Factory();
	ExecuteCommand* command = nullptr;
	if (connectors.front() == "&&") {
	    command = factory->create_command(evalLeft.at(0), ";");
	    And* executable = new And();
	    return executable->execute(command->execute(), factory->create_command(evalRight.at(0), ";"));
	}    
	else if (connectors.front() == "||") { 
	    command = factory->create_command(evalLeft.at(0), ";");
	    Or* executable = new Or();
	    return executable->execute(command->execute(), factory->create_command(evalRight.at(0), ";"));
	}
	else if (connectors.front() == ";") { 
	    command = factory->create_command(evalLeft.at(0), ";");
	    Semi* executable = new Semi();
	    return executable->execute(command->execute(), factory->create_command(evalRight.at(0), ";"));
	}
    }

    // Handle multiple connectors and precedence operators
    int executeCount = 0;
    int openParen = 0;
    int closeParen = 0;
    Factory* factory = new Factory();
    if (connCount > 1) {
	for (unsigned int i = 0; i < userInput.size(); ++i) {
	    if (userInput.at(i) == ")" /* || i == userInput.size() - 1*/) {
		// Done executing, reached the last parentheses in the command sequence or reached
		// the end of the command sequence
		return success;
	    }
	    else if (userInput.at(i) == "(") { // Handle parentheses recursively
		int j = i;
		for (j = i + 1; j < userInput.size(); ++j) {
		    if (userInput.at(j) == "(") {
	    		++openParen;
		    }
		    if (userInput.at(j) == ")") {
			++closeParen;
		    }
		    if (openParen == closeParen - 1) {
			break; // exit for loop when we've encountered the matching )
		    }
		    parentheses.push_back(userInput.at(j));
		}
		i = j;
		Parser* parsed = new Parser();
		if ((connectors.front() == "&&" && success) || (connectors.front() == "||" && !success) || connectors.front() == ";") {
		    success = parsed->execute(parentheses);
		    connectors.pop();
		}
		else {
		    success = parsed->execute(parentheses);
		}
		++executeCount;
		parentheses.clear();
	    }
	    else if (userInput.at(i) != "&&" && userInput.at(i) != "||" && userInput.at(i) != ";") {
		if (executeCount == 0 && connectors.empty()) {
		    evalLeft.push_back(userInput.at(i));
		}
		else {
		    evalRight.push_back(userInput.at(i));
		    if (!connectors.empty()) {
			if (connectors.front() == "&&") {
			    And* executable = new And();
			    success = executable->execute(success, factory->create_command(evalRight.at(0), ";"));
			}
			else if (connectors.front() == "||") {
			    Or* executable = new Or();
			    success = executable->execute(success, factory->create_command(evalRight.at(0), ";"));
			}
			else if (connectors.front() == ";") {
			    Semi* executable = new Semi();
			    success = executable->execute(success, factory->create_command(evalRight.at(0), ";"));
			}
			connectors.pop();
			++executeCount;
			evalRight.clear();
		    }
		}
	    }
	    else if (userInput.at(i) == "&&" || userInput.at(i) == "||" || userInput.at(i) == ";") {
		connectors.push(userInput.at(i));
		if (!evalLeft.empty() && !connectors.empty()) {
		    ExecuteCommand* command = factory->create_command(evalLeft.at(0), ";");
		    success = command->execute();
		    ++executeCount;
		    evalLeft.clear();
		}	
		else if (!evalRight.empty() && !connectors.empty()) {
		    if (connectors.front() == "&&") {
			And* executable = new And();
			success = executable->execute(success, factory->create_command(evalRight.at(0), ";"));
		    }
		    else if (connectors.front() == "||") {
			Or* executable = new Or();
			success = executable->execute(success, factory->create_command(evalRight.at(0), ";"));
		    }
		    else if (connectors.front() == ";") {
			Semi* executable = new Semi();
			success = executable->execute(success, factory->create_command(evalRight.at(0), ";"));
		    }
		    ++executeCount;
		    evalRight.clear();	
		}
	    }
	}	    
    }
    
    return success;
}
    /*
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
    */


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
bool Parser::check_paren(const vector<string> & input) {
    int leftParen = 0;
    int rightParen = 0;
    
    for (unsigned int i = 0; i < input.size(); ++i) {
	for (unsigned int j = 0; j < input.at(i).size(); ++j) {
	    if (input.at(i).at(j) == '(') {
		++leftParen;
	    }
	    if (input.at(i).at(j) == ')') {
		++rightParen;
	    }
	}
    }

    if (leftParen != rightParen) {
	return false;
    }

    return true;
}
vector<string> Parser::eval_precedence(vector<string> & input) {
    vector<string> newCommands;
    stack<char> opStack;
    opStack.push('N');
    queue<string> precQueue;
    queue<string> normQueue;

    int i = 0;
    while (i >= 0 && i < input.size()) {	
	if (input.at(i).find("(") == string::npos) {
	    normQueue.push(input.at(i));
	    ++i;
	}
	else {
	    int k = i;
	    int temp;
	    while (k < input.size()) {
		int j = 0;
		while (j >= 0 && j < input.at(k).size()) {
		    if (input.at(k).at(j) == '(' || input.at(k).at(j) == ')') {
			opStack.push(input.at(k).at(j));
			input.at(k).erase(j, 1);
			j = 0;
		    }
		    else if (input.at(k).find("(") == string::npos && input.at(k).find(")") == string::npos) {
			precQueue.push(input.at(k));
			j = input.at(k).size();
		    }
		    else {
			++j;
		    }
		}
		if (opStack.top() == ')') {
		    while (opStack.top() != '(') {
			while (!precQueue.empty()) {
			    newCommands.push_back(precQueue.front());
			    precQueue.pop();
			}
			opStack.pop();
		    }
		    opStack.pop(); // Pop last '(' off the stack
		    temp = k;
		    k = input.size(); // So we fall out of the while loop
		}
		else {
		    ++k;
		    temp = k;
		}
	    }
	    i = temp + 1;
	}   
    }
    while (!normQueue.empty()) {
	newCommands.push_back(normQueue.front());
	normQueue.pop();	
    }		
    
    return newCommands;    
}
