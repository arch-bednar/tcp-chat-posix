cmake_minimum_required(VERSION 3.28)
project(tcp_chat_posix C)

set(CMAKE_C_STANDARD 99)

add_executable(tcp_chat_posix main.c
        server.c
        client.c
        server.h
        client.h)
