#include "outputredirect.hpp"

using namespace std;

// Constructors
OutRedirect::OutRedirect() : ExecuteCommand() {}
OutRedirect::OutRedirect(ExecuteCommand* input, string output) {
    this->input = input;
    this->output = output;
}

// Execute() function
bool OutRedirect::execute() {
    // Get file descriptor for output file
    int file_desc = open(this->output.c_str(), O_CREAT | O_WRONLY /*|O_TRUNC*/, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP);
    /* Note the definition of the following flags in linux's open() command:
       O_CREAT: if pathname does not exist, it is created as a regular file
       O_WRONLY: set access mode to write only so that file can only be written to
       O_TRUNC: if the file already exists and is a regular file (set by O_CREAT) and access mode allows writing, truncate to length 0
       S_IRUSR: the user has read permission
       S_IRGRP: the group has read permission
       S_IWGRP: the group has write permission
       S_IWUSR: the user has write permission
    */
    if (file_desc < 0) {
	perror("Error opening output file");
	return false;
    }

    int std = dup(1); // Duplicate fileout process
    if(dup2(file_desc, 1) == -1) {
	perror("Error duplicating output file to fileout");
	return false;
    }
    close(file_desc); // Close output file
    dup2(file_desc, 1); // Copy file_desc to fileout (1)
    this->input->execute(); // Execute the passed in command
    close(std);
    dup2(std, 1);

    return true;
}

// Inherited functions, set them to doing nothing for now
void OutRedirect::print_command() {}
string OutRedirect::get_command() {}
string OutRedirect::get_command_full() {}
string OutRedirect::get_separator() {}

