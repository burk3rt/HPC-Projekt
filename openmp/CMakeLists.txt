cmake_minimum_required(VERSION 3.23)
#project(HPC_Projekt C)
project(HPC_Projekt C )

set(CMAKE_C_STANDARD 11)

add_executable(HPC_Projekt
        ant.c
        ../cities/5/cities5.c
        ../cities/csv-input.c
        ../cities/csv-input.h)
