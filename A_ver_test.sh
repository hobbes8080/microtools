#!/bin/bash

# this hack uses export of a variable from parent to child and uses the output from the child as the value. thus, passing a variable from child to parent is achieved, which is otherwise impossible.
export ver=$(./A_ver.sh)

echo This is the content of ver: $ver
