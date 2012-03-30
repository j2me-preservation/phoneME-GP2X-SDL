#!/bin/sh

SDK=/home/baro/dev/caanoo/GPH_SDK
PREFIX=$SDK/tools/gcc-4.2.4-glibc-2.7-eabi
TARGET=arm-gph-linux-gnueabi

#export LD_LIBRARY_PATH=/toolchain/arm-openwiz-linux-gnu/lib

#PATH="$PREFIX:$PREFIX/bin:$PREFIX/$TARGET/bin:$PATH"
#export PATH

export LDFLAGS=-L/$SDK/lib/target
export CPPFLAGS=-I/$SDK/include
export CXXFLAGS=-I/$SDK/include
export CFLAGS=-I/$SDK/include


#PATH="$PREFIX:$PREFIX/bin:$PREFIX/$TARGET/bin:$PATH"
#export PATH

exec make $*
