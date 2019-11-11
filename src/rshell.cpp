#include <iostream>
#include "executecommand.hpp"
#include "executegroup.hpp"
#include "execute.hpp"
#include "parser.hpp"
#include <string>

using namespace std;

int main() {
    string userInput;
    getline(cin, userInput);
    Parser* parsed = new Parser();
    ExecuteGroup* executable = parsed->parse(userInput);
    executable->print_command();

    return 0;
}
