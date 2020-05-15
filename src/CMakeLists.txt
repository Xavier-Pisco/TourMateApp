cmake_minimum_required(VERSION 3.16)
project(src)

set(CMAKE_CXX_STANDARD 14)

add_executable(src main.cpp lib/GraphViewer/graphviewer.cpp lib/GraphViewer/graphviewer.h lib/GraphViewer/connection.cpp lib/GraphViewer/connection.h graph/GraphClasses.cpp graph/GraphClasses.h Application.cpp Application.h input/Converter.cpp input/Converter.h input/UserInput.cpp input/UserInput.h graph/Position.h graph/RoadInterceptionFromTxt.h User.cpp User.h)
