#!/usr/bin/env bash

ORIGIN_DIR=`pwd`
if [ -d "build" ]; then 
  rm -rf "build"
fi
mkdir -p "install"
mkdir "build" ; cd "build"
cmake ..\
    -DCMAKE_INSTALL_PREFIX:FILEPATH=${ORIGIN_DIR}/install \
    -DUSE_STD_PARALLEL:BOOL=ON \
    -DUSE_OPENMP:BOOL=OFF \
    -DCMAKE_BUILD_TYPE:STRING=Release
make -j4 VERBOSE=1
make install
cd ${ORIGIN_DIR}
