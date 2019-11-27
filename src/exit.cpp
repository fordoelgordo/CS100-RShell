#include "exit.hpp"

using namespace std;

// Constructor (utilize abstract base class)
Exit::Exit() : ExecuteCommand() {}

// Public functions
bool Exit::execute() {
    exit(0);
    return true;
}
void Exit::print_command() {
    cout << "exit()" << endl;
}
string Exit::get_command() {
    string temp = "exit";
    return temp;
}
string Exit::get_command_full() {
    string temp = "exit";
    return temp;
}
string Exit::get_separator() {
    string temp = ";";
    return temp;
}
