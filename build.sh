#!/bin/bash

# version markers location:
# sinlge main location to set version manually: A_ver.sh
# build.sh
# A_ver.h
# microtools (bash script)

# this hack uses export of a variable from parent to child and uses the output from the child as the value. thus, passing a variable from child to parent is achieved, which is otherwise impossible.
# another twist: in microtools, use installed version of this script. in build.sh, use local version.
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
    cp ./microtools_$ver-1_amd64.deb ./microtools_development/deb-microtools/

    # compile docker image
    # TODO....
fi
