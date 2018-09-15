#!/bin/bash

gcc --std=c99 server.c open62541/build/open62541.c -o server
gcc --std=c99 client.c open62541/build/open62541.c -o client
