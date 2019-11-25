#!/bin/bash

../rshell > "exit #test"
../rshell > "exit #comment1 #comment2"
../rshell > "ls; exit"
../rshell > "echo hello; mkdir test; rm -r test && exit"
../rshell > "exit"
