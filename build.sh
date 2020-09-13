#!/bin/sh
make clean
gcc client.c -lm -o client
gcc server.c -lm -o server

