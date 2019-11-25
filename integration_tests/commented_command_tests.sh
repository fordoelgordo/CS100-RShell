#!/bin/bash

..rshell < "ls # hello"
../rshell < "ls -a; echo hello # ignore after this"
../rshell < "echo world && ls #; git status"
../rshell < "echo \"Hi this is a test\" #testing 1 2 3 4"
../rshell < "exit"
