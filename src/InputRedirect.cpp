#include "InputRedirect.hpp"

// "<" 

InputRedirect::InputRedirect(ExecuteCommand *l, ExecuteCommand *r) {
        this->left = l;
        this->right = r;
}

bool InputRedirect::execute() {

        int std = dup(0);

        int open_file = open(this->right, O_RDWR | O_CREAT);

        if(open_file < 0) {
                perror("Error opening Out file");
                return false;
        }

        dup2(open_file, 1);
        close(open_file);

        bool process = left->execute();

        dup2(open_file, 0);
        close(0);

        return process;
}

void InputRedirect::set_left(ExecuteCommand *l) {
        this->left = l;
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

