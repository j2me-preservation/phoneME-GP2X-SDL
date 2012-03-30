#!/bin/bash

#SDKBASE = /home/baro/dev/caanoo/GPH_SDK
#TOOLBASE = ${SDKBASE}/tools/gcc-4.2.4-glibc-2.7-eabi/arm-gph-linux-gnueabi/bin/arm-gph-linux-gnueabi-
#TARGET = arm-linux

#export GNU_TOOLS_DIR = /home/baro/dev/caanoo/GPH_SDK/tools/gcc-4.2.4-glibc-2.7-eabi/arm-gph-linux-gnueabi/bin

#CC = ${TOOLBASE}gcc
#CXX = ${TOOLBASE}g++

export MEHOME=~/dev/caanoo/phoneME-SDL-1.0.1-src/phoneme_feature
mkdir $MEHOME/build_output 
export BUILD_OUTPUT_DIR=$MEHOME/build_output
#export JDK_DIR=/usr/lib/jvm/j2sdk1.4.2_16
export JDK_DIR=/root/dev/j2sdk1.4.2_19
export PATH=$JDK_DIR/bin:$PATH
# /home/baro/dev/caanoo/GPH_SDK/tools/gcc-4.2.4-glibc-2.7-eabi/arm-gph-linux-gnueabi
#export MONTAVISTA=/gp2xsdk/Tools/arm-gp2x-linux
#export MONTAVISTA=/home/baro/dev/caanoo/GPH_SDK/tools/gcc-4.2.4-glibc-2.7-eabi/arm-gph-linux-gnueabi
export MONTAVISTA=/root/dev/caanoo/GPH_SDK/tools/gcc-4.2.4-glibc-2.7-eabi
export PCSL_OUTPUT_DIR=$BUILD_OUTPUT_DIR/pcsl
export PCSL_PLATFORM=linux_i386_gcc

echo --------------------------------i386 building NETWORK_MODULE $MEHOME/pcsl
cd $MEHOME/pcsl
make  NETWORK_MODULE=bsd/generic $@ || exit 1


echo --------------------------------arm building NETWORK_MODULE $MEHOME/pcsl
export PCSL_PLATFORM=linux_arm_gcc
cd $MEHOME/pcsl

#SDK=/home/baro/dev/caanoo/GPH_SDK
#PREFIX=$SDK/tools/gcc-4.2.4-glibc-2.7-eabi
#TARGET=arm-gph-linux-gnueabi

#PATH="$PREFIX:$PREFIX/bin:$PREFIX/$TARGET/bin:$PATH"
#export PATH
#export LDFLAGS=-L/$SDK/lib/target
#export CPPFLAGS=-I/$SDK/include
#export CXXFLAGS=-I/$SDK/include
#export CFLAGS=-I/$SDK/include
#echo $PATH

#make  NETWORK_MODULE=bsd/generic GNU_TOOLS_DIR=$MONTAVISTA
make  NETWORK_MODULE=bsd/generic GNU_TOOLS_DIR=$MONTAVISTA $@ || exit 1
#make  NETWORK_MODULE=bsd/qte GNU_TOOLS_DIR=$MONTAVISTA $@ || exit 1

export JVMWorkSpace=$MEHOME/cldc
export JVMBuildSpace=$BUILD_OUTPUT_DIR/cldc
echo -------------------------------- arm building JVMWORKSPACE
cd $JVMWorkSpace/build/linux_arm
make ENABLE_PCSL=true PCSL_OUTPUT_DIR=$BUILD_OUTPUT_DIR/pcsl ENABLE_ISOLATES=true GNU_TOOLS_DIR=$MONTAVISTA ENABLE_COMPILATION_WARNINGS=true $@ || exit 1

echo -------------------------------- i386 building JVMWORKSPACE
cd $JVMWorkSpace/build/linux_i386
export PCSL_PLATFORM=linux_i386_gcc
make ENABLE_PCSL=true PCSL_OUTPUT_DIR=$BUILD_OUTPUT_DIR/pcsl ENABLE_ISOLATES=true ENABLE_COMPILATION_WARNINGS=true $@ || exit 1

export MIDP_OUTPUT_DIR=$BUILD_OUTPUT_DIR/midp

echo --------------------- i386 building SDL
cd $MEHOME/midp/build/linux_sdl_gcc
make USE_SDL_ABB=true PCSL_OUTPUT_DIR=$BUILD_OUTPUT_DIR/pcsl CLDC_DIST_DIR=$BUILD_OUTPUT_DIR/cldc/linux_i386/dist TOOLS_DIR=$MEHOME/tools USE_MULTIPLE_ISOLATES=true $@ || exit 1

echo --------------------  arm building SDL
cd $MEHOME/midp/build/linux_sdl_gcc
make USE_SDL_ABB=true PCSL_OUTPUT_DIR=$BUILD_OUTPUT_DIR/pcsl CLDC_DIST_DIR=$BUILD_OUTPUT_DIR/cldc/linux_arm/dist TOOLS_DIR=$MEHOME/tools TARGET_CPU=arm USE_MULTIPLE_ISOLATES=true GNU_TOOLS_DIR=$MONTAVISTA $@ || exit 1

