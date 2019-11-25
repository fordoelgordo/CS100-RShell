#include "testexecute.hpp"

using namespace std;

// Test class constructors
TestExecute::TestExecute() : ExecuteCommand() {this->executeSuccess = false;}
TestExecute::TestExecute(vector<string> enteredCommand, string sep) {
    this->command = enteredCommand;
    this->separator = sep;
}

// Test class execute() function
void TestExecute::execute() {
    // Implement the test command logic
    int pathStatus; // stat() function returns 0 if successful, e.g. path exists, otherwise returns -1 
    struct stat buf; // information about the path status is stored in this variable
    bool eflag = false;
    bool fflag = false;
    bool dflag = false;   
    string path;    

    // Parse out the ending bracket that may be passed in if [ <arguments> ] is entered 
    for (unsigned int i = 0; i < this->command.size(); ++i) {
	if (this->command.at(i) == "]") {
	    this->command.pop_back();
	}
    }
    for (unsigned int i = 0; i < this->command.size(); ++i) {
	if (this->command.at(i).find("]") != string::npos) {    
	    this->command.at(i) = regex_replace(this->command.at(i), regex(" +$"), "");
	}
    }
    
    // The passed command should be a vector of strings starting with "test" or "[", so check for
    // flags and the path to search for from indices 1 - size() - 1
    for (unsigned int i = 1; i < this->command.size(); ++i) {
	if (this->command.at(i) == "-e") {
	    eflag = true;
	}
	else if (this->command.at(i) == "-f") {
	    fflag = true;
	}
	else if (this->command.at(i) == "-d") {
	    dflag = true;
	}
	else if (this->command.at(i) != "") {
	    path = this->command.at(i);
	}
    }

    // Check to see if any of the allowed flags were passed.  If they were not, then default to "-e"
    if (!eflag && !fflag && !dflag) {
	eflag = true;
    }
    
    // Convert the path argument to char* as this is the argument type that stat() takes
    char argument[path.size() + 1];
    strcpy(argument, path.c_str());
    
    pathStatus = stat(argument, &buf);    
    // Check the return value of stat() based on the flags entered by the user
    if (pathStatus == -1) { // Directory is not found
	cout << "(False)" << endl;
	this->executeSuccess = false;
    }
    else {
	if (eflag) {
	    cout << "(True)" << endl;
	    this->executeSuccess = true;
	}
	else if (fflag) {
	    if(S_ISREG(buf.st_mode)) { // S_ISREG returns non-zero if path points to a regular file
		cout << "(True)" << endl;
		this->executeSuccess = true;
	    }
	    else {
		cout << "(False)" << endl;
		this->executeSuccess = false;
	    }
	}
	else if (dflag) {
	    if(S_ISDIR(buf.st_mode)) { // S_ISDIR returns non-zero if path points to a directory
		cout << "(True)" << endl;
		this->executeSuccess = true;
	    }
	    else {
		cout << "(False)" << endl;
		this->executeSuccess = false;
	    }
	}
	this->executeSuccess = true; // If none of the nested branches are taken, but the main else branch is taken then return true
    }
}

// Implement the rest of the class inherited functions from base class ExecuteCommand
void TestExecute::print_command() {
    for (unsigned int i = 0; i < this->command.size() - 1; ++i) {
	cout << this->command.at(i) << " ";
    }
    cout << this->command.at(this->command.size() - 1) << endl;
}
string TestExecute::get_command() {
    string val;
    if (this->command.at(0) == "test") {
	val = "test";
    }
    else {
	val = "[ ]";
    }
    return val;
}
string TestExecute::get_command_full() {     
    string stringCommand;
    for (unsigned int i = 0; i < this->command.size() -1; ++i) {
	stringCommand += this->command.at(i) + " ";
    }
    stringCommand += this->command.at(this->command.size() - 1);

    return stringCommand;
}
string TestExecute::get_separator() {
    return this->separator;
}
bool TestExecute::get_success() {
    return this->executeSuccess;
}
