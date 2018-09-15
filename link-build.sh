#!/bin/bash

MPWD=$(pwd)
LIB_DIR=${MPWD}/open62541
BUILD_DIR=${LIB_DIR}/build

cd ${LIB_DIR}
sudo rm -r ${BUILD_DIR}
mkdir ${BUILD_DIR}
cd ${BUILD_DIR}

cmake -DBUILD_SHARED_LIBS=ON \
-DUA_ENABLE_AMALGAMATION=ON \
-DUA_ENABLE_FULL_NS0=ON \
-DUA_LOGLEVEL=100 \
-DUA_DEBUG=ON \
-DUA_DEBUG_DUMP_PKGS=ON ..

make -j9
sudo make install

cd ${MPWD}
# gcc --std=c99 -o server server.c -I ./open62541/build -L ./open62541/build/bin/ -lopen62541
# gcc --std=c99 -o client client.c -I ./open62541/build -L ./open62541/build/bin/ -lopen62541

gcc --std=c99 -o  ${MPWD}/server  ${MPWD}/server.c -lopen62541
gcc --std=c99 -o  ${MPWD}/client  ${MPWD}/client.c -lopen62541

./server
