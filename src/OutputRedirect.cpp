#include "OutputRedirect.hpp"

// ">" ">>"

OutputRedirect::OutputRedirect(string left, ExecuteCommand *r) {
        this->left = l;
        this->right = r;
}

bool OutputRedirect::execute() {

        int std = dup(1);
				//need to fix this->left(just there for placement)
        int open_file = open(this->left, O_RDWR | O_CREAT);

        if(open_file < 0) {
                perror("Error opening Out file");
                return false;
        }

        dup2(open_file, 1);
        close(open_file);

        bool process = left->execute();

        dup2(open_file, 1);
        close(1);

        return process;
}

void OutputRedirect::set_left(string left) {
        this->left = l;
}

void OutputRedirect::set_right(ExecuteCommand *r) {
        this->right = r;
}

void OutputRedirect::print_command() {

}

string OutputRedirect::get_command_full {

}

string OutputRedirect::get_seperator() {

}

