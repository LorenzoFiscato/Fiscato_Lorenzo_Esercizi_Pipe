cmake_minimum_required(VERSION 4.2)
project(EsPipe)

set(CMAKE_CXX_STANDARD 20)

add_executable(Main main.cpp)
add_executable(EsPipe EsPipe.c)
add_executable(Es1 Esercizi/Es1.c)
add_executable(Es2 Esercizi/Es2.c)
add_executable(Es3 Esercizi/Es3.c)