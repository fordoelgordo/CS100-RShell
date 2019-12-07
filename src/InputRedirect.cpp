#include "InputRedirect.hpp"

// "<" 

InputRedirect::InputRedirect(string left, ExecuteCommand *r) {
        this->left = left;
        this->right = r;
}

bool InputRedirect::execute() {

        int std = dup(0);

        int open_file = open(this->left, O_RDWR | O_CREAT);

        if(open_file < 0) {
                perror("Error opening Out file");
                return false;
        }

        dup2(open_file, 1);
        close(open_file);

        bool process = this->right->execute();

        dup2(open_file, 0);
        close(0);

        return process;
}

void InputRedirect::set_left(string left) {
        this->left = left;
}

void InputRedirect::set_right(ExecuteCommand *r) {
        this->right = r;
}

void InputRedirect::print_command() {

}

string InputRedirect::get_command_full {

}

string InputRedirect::get_seperator() {

}

