#!/bin/bash

# gcc --std=c99 -o server -I ./open62541/build server.c open62541/build/open62541.c 
# gcc --std=c99 -g -o client -I ./open62541/build client.c open62541/build/open62541.c 
# gcc --std=c99 -g -o client2 -I ./open62541/build client2.c open62541/build/open62541.c 

gcc --std=c99 -g -o client3 -I ./open62541/build clinet3.c open62541/build/open62541.c 
