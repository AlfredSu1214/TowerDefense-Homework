#!/bin/sh

WORKDIR=$(pwd)

mkdir build && cd build && cmake .. && make

cd $WORKDIR

ln -sf resources build/resources

$WORKDIR/build/TowerDefense
