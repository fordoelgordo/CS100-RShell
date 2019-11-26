#include "parser.hpp"
#include "factory.hpp"

using  namespace std;

Parser::Parser() {}
void Parser:: parse(string userInput) {
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
 
    // Parse out parentheses, place on separate lines
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
   
    // Evaluate connectors 
    int leftParen = 0;
    int rightParen = 0;
    int connCount = 0;
    vector<string> evalLeft;
    vector<string> evalRight;
    vector<string> parentheses;
    queue<string> connectors;

    for (unsigned int i = 0; i < finalCommands2.size(); ++i) {
	if (finalCommands2.at(i) == "(") {
	    ++leftParen;
	}
	else if (finalCommands2.at(i) == ")") {
	    ++rightParen;
	}
	else if (finalCommands2.at(i) == "&&" || finalCommands2.at(i) == "||" || finalCommands2.at(i) == ";") {
	    ++connCount;
	}
    }

    // Conditional branch to check for correct parentheses, exit parsing if not
    // Need to update the parentheses check function
 
    // No connectors present in command sequence
    if (connCount == 0) {
	Factory* factory = new Factory();
	ExecuteCommand* executable = nullptr;
	if (finalCommands2.at(0) == "(") {
	    executable = factory->create_command(finalCommands2.at(1), ";");
	}
	else {
	    executable = factory->create_command(finalCommands2.at(0), ";");
	}
	executable->execute();
    }
     
    // Handle a single connector 
    if (connCount == 1) {
	// Loop through the commands, add connectors to connector queue
	for (unsigned int i = 0; i < finalCommands2.size(); ++i) {
	    if (finalCommands2.at(i) == "(" || finalCommands2.at(i) == ")") {
		// Do nothing, precedence does not matter in this situation
	    }
	    else if (finalCommands2.at(i) != "&&" && finalCommands2.at(i) != "||" && finalCommands2.at(i) != ";") {
		if (connectors.empty()) {
		    evalLeft.push_back(finalCommands2.at(i));
		}
		else {
		    evalRight.push_back(finalCommands2.at(i));
		}
	    }
	    else {
		// Add connector to the queue
		connectors.push(finalCommands2.at(i));
	    }
	}
	bool success;
	Factory* factory = new Factory();
	ExecuteCommand* command = nullptr;
	if (connectors.front() == "&&") {
	    command = factory->create_command(evalLeft.at(0), ";");
	    And* executable = new And();
	    executable->execute(command->execute(), factory->create_command(evalRight.at(0), ";"));
	}    
	else if (connectors.front() == "||") { 
	    command = factory->create_command(evalLeft.at(0), ";");
	    Or* executable = new Or();
	    executable->execute(command->execute(), factory->create_command(evalRight.at(0), ";"));
	}
	else if (connectors.front() == ";") { 
	    if (evalLeft.at(0).substr(0,4) == "test" || evalLeft.at(0).substr(0,1) == "[") {
		TestExecute* test = new TestExecute(create_stringvec(evalLeft.at(0)), connectors.front());
		success = test->execute();
	    }
	    else {
		Execute* command = new Execute(create_charstar(evalLeft.at(0)), connectors.front());
		success = command->execute();
	    }
	    // Execute right branch no matter what when the connector is a ;
	    if (evalRight.at(0).substr(0,4) == "test" || evalRight.at(0).substr(0,1) == "[") {
		TestExecute* rtest = new TestExecute(create_stringvec(evalRight.at(0)), ";");
		rtest->execute();
	    }
	    else {
		Execute* rcommand = new Execute(create_charstar(evalRight.at(0)), ";");
		rcommand->execute();
	    }
	}
    }

    /*
    // Handle multiple connectors and precedence
    int excecuteCount = 0;
    if (connCount > 1) {
	bool success;
	for (unsigned int i = 0; i < finalCommands2.size(); ++i) {
	    if (finalCommands2.at(i) == ")" && i == finalCommands2.size() - 1) {
		// Done executing, reached the last parentheses in the command sequence
	    }
	    else { // Executing commands left in the commands vector
		if (executeCount == 0 && connectors.empty()) { // Encountered first command to execute
		    evalLeft.push_back(finalCommands2.at(i));
		}
		else {
		    evalRight.push_back(finalCommands2.at(i));
		    if (!connectors.empty()) {
			if (connectors.size() >= 2) {
			    connectors.pop();
			}
			if (i == finalCommands2.size() - 1) {
			    if (connectors.front() == "&&") {				
				if (evalLeft.at(0).substr(0,4) == "test" || evalLeft.at(0).substr(0,1) == "[") {
				    TestExecute* test = new TestExecute(create_stringvec(evalLeft.at(0)), connectors.front());
				    success = test->execute();
				}
				else {
				    Execute* command = new Execute(create_charstar(evalLeft.at(0)), connectors.front());
				    success = command->execute();
				}
				// Only execute right branch if left branch succeeds
				if (success) {			
				    if (evalRight.at(0).substr(0,4) == "test" || evalRight.at(0).substr(0,1) == "[") {
					TestExecute* rtest = new TestExecute(create_stringvec(evalRight.at(0)), ";");
					rtest->execute();
				    }
				    else {
					Execute* rcommand = new Execute(create_charstar(evalRight.at(0)), ";");
					rcommand->execute();
				    }
				}
		 	    }
			    else if (connectors.front() == "||") { 	
				if (evalLeft.at(0).substr(0,4) == "test" || evalLeft.at(0).substr(0,1) == "[") {
				    TestExecute* test = new TestExecute(create_stringvec(evalLeft.at(0)), connectors.front());
				    success = test->execute();
				}
				else {
				    Execute* command = new Execute(create_charstar(evalLeft.at(0)), connectors.front());
				    success = command->execute();
				}
				// Only execute right branch if left branch fails
				if (!success) {			
				    if (evalRight.at(0).substr(0,4) == "test" || evalRight.at(0).substr(0,1) == "[") {
					TestExecute* rtest = new TestExecute(create_stringvec(evalRight.at(0)), ";");
					rtest->execute();
				    }
				    else {
					Execute* rcommand = new Execute(create_charstar(evalRight.at(0)), ";");
					rcommand->execute();
				    }
				}
			    }	
			    else if (connectors.front() == ";") {    
				if (evalLeft.at(0).substr(0,4) == "test" || evalLeft.at(0).substr(0,1) == "[") {
				    TestExecute* test = new TestExecute(create_stringvec(evalLeft.at(0)), connectors.front());
				    success = test->execute();
				}
				else {
				    Execute* command = new Execute(create_charstar(evalLeft.at(0)), connectors.front());
				    success = command->execute();
				}
				// Execute right branch no matter what
				if (evalRight.at(0).substr(0,4) == "test" || evalRight.at(0).substr(0,1) == "[") {
				    TestExecute* rtest = new TestExecute(create_stringvec(evalRight.at(0)), ";");
				    rtest->execute();
				}
				else {
				    Execute* rcommand = new Execute(create_charstar(evalRight.at(0)), ";");
				    rcommand->execute();
				}
			    }
			    connectors.pop();
			    evalRight.clear();
			    ++executeCount;
			}
		    }
		}		
	    }
	}   
    */
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
