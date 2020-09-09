cmake_minimum_required(VERSION 3.16)
project(EE542_Lab3_FastReliableFileTransfer C)

set(CMAKE_C_STANDARD 99)

add_library(EE542_Lab3_FastReliableFileTransfer ./client/client.c ./server/server.c)