#!/bin/bash

../rshell < "ls -a;"
../rshell < "echo hello"
../rshell < "mkdir test"
../rshell < "echo \"Hello World\" # && echo test"
../rshell < "git status;"
../rshell < "ls # -lR"
../rshell < "touch test.cpp # test"
../rshell < "rm test.cpp"
../rshell < "exit"
