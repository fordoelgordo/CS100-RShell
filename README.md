# CS 100 Programming Project
Fall 2019  
Ford St. John, 862125078  
Tanner Cohen

## Introduction
This program will be able to generate an executable that when exercised takes in user input and executes any of the executable programs located in the PATH environment variable locations.  The composite design pattern will be followed, meaning that the client will have a simple and intuitive interface to work with that will hide all unnecessary functional implmentation from the user.    


## Prototypes/Research
We created a prototype user input parsing function that parses user input into executable commands.  The prototype was implemented utilizing the Tokenizer class in the c++ boost library.  If the user enters the command:
```shell
$ ls -a; echo hello && mkdir test || echo world; git status
```
The function will parse the commands into the following:
```shell
$ ls -a
$ echo hello
$ mkdir test
$ echo world
$ git status
```
The parser can be compiled with the following command:
```shell
$ g++ -g -w -Wall -Werror parser.cpp
```
The parser also saves the connectors (; && ||) so that the commands can be properly executed (e.g. all commands followed by ; are executed, commands followed by && are executed, with the command following the && executed if and only if the first command succeeded, and commands followed by || are always executed with the command followed by || executed if and only if the first command failed).
