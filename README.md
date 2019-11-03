# PetitPoint

Just a c++ project I do for fun.

## Install prerequesite for build
sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev

## Build
cd build
cmake ..
make

build with debugging info (for gdb):
cmake -DCMAKE_BUILD_TYPE=Debug ..

## Run
bin/PetitPoint
