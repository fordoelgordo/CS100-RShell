#include "inputredirect.hpp"

using namespace std;

// Constructors
InRedirect::InRedirect() : ExecuteCommand() {}
InRedirect::InRedirect(ExecuteCommand* input, string output) {
    this->input = input;
    this->output = output;
}

// Execute() function
bool InRedirect::execute() {
    int file_desc = open(output.c_str(), O_RDONLY); // Open output file with read only access
    if (file_desc < 0) {
	perror("Error opening input file");
	return false;
    }

    int std = dup(0); // Duplicate filein process
    if (dup2(file_desc, 0) == -1) {
	perror("Error duplicating input file to filein");
	return false;
    }
    close(file_desc); // Close input file
    dup2(file_desc, 0); // Copy file_desc to filein (0)
    this->input->execute();
    close(std);
    dup2(std, 0);
    
    return true;
}

// Inherited functions, set them to do nothing for now
void InRedirect::print_command() {}
string InRedirect::get_command() {}
string InRedirect::get_command_full() {}
string InRedirect::get_separator() {}
