#!/bin/bash

version="72"
text="2024-12 by Matthias P Hilty"

# this is the counterpart to the hack that uses export of a variable from parent to child and uses the output from the child as the value. thus, passing a variable from child to parent is achieved, which is otherwise impossible.
echo $version

# this script sets the version variable that is used in microtools (bash script) and build.sh
# additionally, it modifies A_ver.h that is used to set the version number within the cpp code

# exchange the line in A_ver.h that contains the vaer definition... does not fully work
# sed -i -e "s/define ver_def \"*\"/define ver_def \"$ver\"/g" ./A_ver.h

# generate a new A_ver.h with all the required information. do this only if called with the -h option (which is used by build.sh, but not by microtools)
opts_string=[h]
ARGS=$@
OPTS=`getopt -o "$opts_string" -- "$@"`
eval set -- "$OPTS"
while true; do
    case "$1" in
	-h)
	    echo -e "/* microtools version declaration */\n\n#define ver_def \""$version"\"\n#define ver_txt \""$text"\"" > ./version.h
	    echo "Generated microtools version header file."
	    exit
	    shift;;
	--)
	    break;;
	*)
	    break;;
    esac
done
