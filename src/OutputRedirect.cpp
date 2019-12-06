#include "OutputRedirect.hpp"

// ">" ">>"

OutputRedirect::OutputRedirect(ExecuteCommand *l, string right) {
        this->left = l;
        this->right = r;
}

/*OutputRedirect::OutputRedirect(ExecuteCommand *l, ExecuteCommand *r) {
        this->left = l;
        this->right = r;
}*/

bool OutputRedirect::execute() {

        int std = dup(1);
				//need to fix this->right(just there for placement)
        int open_file = open(this->right, O_RDWR | O_CREAT);

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

void OutputRedirect::set_left(ExecuteCommand *l) {
        this->left = l;
}

void OutputRedirect::set_right(string right) {
	this->right = r;
}
/*void OutputRedirect::set_right(ExecuteCommand *r) {
        this->right = r;
}*/

void OutputRedirect::print_command() {

}

string OutputRedirect::get_command_full {

}

string OutputRedirect::get_seperator() {

}

