cmake_minimum_required(VERSION 3.16)
project(src)

set(CMAKE_CXX_STANDARD 14)

add_executable(src main.cpp graph/GraphClasses.cpp graph/GraphClasses.h Application.cpp Application.h input/Converter.cpp input/Converter.h)