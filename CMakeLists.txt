cmake_minimum_required(VERSION 3.22)
project(IPC_ C)

set(CMAKE_C_STANDARD 11)

add_executable(IPC_
        namedpipe_server.c namedpipe_client.c pipe.c shared_setter.c shared_show.c UDS_client.c UDS_server.c)
