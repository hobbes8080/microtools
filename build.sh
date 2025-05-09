#!/bin/bash

# version markers location:
# sinlge main location to set version manually: A_ver.sh
# build.sh
# A_ver.h
# microtools (bash script)

# CALL this file with -d to also initiate the docker build

# get commend line arguments
opts_string=[d]
ARGS=$@
OPTS=`getopt -o "$opts_string" -- "$@"`
eval set -- "$OPTS"



arch="amd64"

# this hack uses export of a variable from parent to child and uses the output from the child as the value. thus, passing a variable from child to parent is achieved, which is otherwise impossible.
# another twist: in microtools, use installed version of this script. in build.sh, use local version.
echo "Building procedure initiated for microtools version:"
export ver=$(./A_ver.sh)

# generate A_ver.h that sets the version for the cpp files.
./A_ver.sh -h

# cmake
cmake .
# make
make -j12



# only proceed to build and installation of .deb if make has succeeded
if [ $? -eq 0 ]; then
    # build and install .deb
    checkinstall --pkgname microtools --pkgversion $ver -y --default
    # copy .deb within internal repository directory structure in order to compile docker image
    cp ./microtools_$ver-1_$arch.deb ./microtools_development/deb-microtools/


    # process command line arguments
    while true; do
        case "$1" in
        -d)
            echo "Initiating Docker Build."
            # compile docker image
            ./microtools_development/docker-microtools/build.sh
            exit
            shift;;
        --)
            break;;
        *)
            break;;
        esac
    done

fi
