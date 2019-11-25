#!/bin/bash

../rshell << EOF
(echo hello world)
(echo A && echo B) || (echo C && echo D)
((echo A && echo B) && echo C)
echo A || (echo B && echo C)
echo A && (echo B || echo C)
(echo hello && echo world) && (echo !! || echo test)
(ls -a && (echo hello world && echo file))
exit
EOF

