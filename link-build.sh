#!/bin/bash

gcc --std=c99 -o server server.c -I . -L ./open62541/build/bin/ -lopen62541
gcc --std=c99 -o client client.c -I . -L ./open62541/build/bin/ -lopen62541
