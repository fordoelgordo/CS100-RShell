# CS 100 Programming Project
Fall 2019  
Ford St. John, 862125078  
Jeffreyson Nguyen, 862154834

## Introduction
The compiled code generates an executable "rshell" that emulates command shell functionality.  The rshell currently supports commands availabile through the PATH variable, the "test" command (and it's symbolic equivalent []) along with user-passed flags which test for file and directory existence, precedence evaluation using parentheses, and chained execution of supported commands using the connectors &&, || and ;.  As with a normal command shell, users can enter any number of commands in infinitum until "exit" is entered which effectively exits the shell.  Initial input validation is only performed on the correct passing of precedence operators.  Otherwise, invalid user input is accepted (e.g un-executable commands) at which point the program will terminate upon failure to excute.

## Diagram
![OMT Diagram](https://github.com/cs100/assignment-git_shorty_assn/blob/master/images/Rshell%20OMT%20.png)

## Classes
* ExecuteCommand
  * Abstract base class that holds a virtual void execute() = 0 public function
* Exit
  * Child of ExecuteCommand, implements the "exit" functionality so that when the user enters "exit" the shell executable ends
* Execute
  * Child of ExecuteCommand, implements all executables associated with the PATH environment variable utilizing execvp(), waitpid() and fork()
* TestExecute
  * Child of ExecuteCommand, implements the "test" functionality which tests for the existence of a file or directory utilizing the stat() function.  This class also implements the abstract command "[]" which the user can pass to instantitate a "test" command.  This class checks for user-passed flags (-e, -d, -f) and analyzes the existence of the file or directory accordingly utilizing the S_ISREG and S_ISDIR functions.  Assumes that if no flag is expressely passed, default to the -e flag
* Parser
  * Has parse() method which parses a user-entered string and returns a vector<string> which the ExecuteCommand subclasses can utilize to execute commands.  The parse method accounts for the precedence operators, comments (#), quotation marks (" ") and connectors (&& || ;)
  * Has execute() method which takes the vector<string> of parsed user input and executes the entered commands
* Factory
  * Child of Parser so that it can inherit some protected methods utilized in command processing.  Creates the appropriate ExecuteCommand object based on the parsed user input
* And
  * Holds an ExecuteCommand* reference and implements the logic of two commands separated by the connector && in which the "right" command of structure leftCommand && rightCommand is executed if an only if leftCommand's execution evaluates to true
* Or
  * Holds an ExecuteCommand* reference and implements the logic of two commands separated by the connector ||.  Or will execute rightCommand in the structure leftCommand || rightCommand if and only if leftCommand's execution evaluates to false
* Semi
  * Holds an ExecuteCommand* reference and implements the logic of two commands separated by the connector ;.  Semi will execute rightCommand in the structure leftCommand ; rightCommand regardless if leftCommand's execution evaluates to true or false
 
