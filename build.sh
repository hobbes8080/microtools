#!/bin/bash

ver="63"

# cmake
cmake .
# make
make -j12
# build and install .deb
checkinstall --pkgname microtools --pkgversion $ver -y --default
# copy .deb within internal repository directory structure in order to compile docker image
cp ./microtools_63-1_amd64.deb ./microtools_development/deb-microtools/

# compile docker image
# TODO....
