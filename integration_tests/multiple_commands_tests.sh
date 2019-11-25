#!/bin/bash

../rshell < "ls -a; echo hello && mkdir test"
../rshell < "echo world && echo hello || echo test"
../rshell < "echo world && echo hello # echo dont echo"
../rshell < "touch test.cpp || echo hello"
../rshell < "ls && rm test.cpp; mkdir test"
../rshell < "rm -r test || echo cleared && ls; exit"
