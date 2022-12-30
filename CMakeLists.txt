cmake_minimum_required(VERSION 3.24)
project(HPC_Projekt C)

set(CMAKE_C_STANDARD 11)

add_executable(HPC_Projekt
        ant.c
        cities/5/cities5.c
        cities/10/cities10.c
        )
