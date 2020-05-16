cmake_minimum_required(VERSION 3.16)
project(src)

set(CMAKE_CXX_STANDARD 14)

add_executable(src main.cpp lib/GraphViewer/graphviewer.cpp lib/GraphViewer/graphviewer.h lib/GraphViewer/connection.cpp lib/GraphViewer/connection.h graph/GraphClasses.cpp graph/GraphClasses.h application/Application.cpp application/Application.h input/Converter.cpp input/Converter.h input/UserInput.cpp input/UserInput.h graph/Position.h application/User.cpp application/User.h graph/algorithms/dijkstra.h application/RouteMaker.cpp application/RouteMaker.h graph/POI.cpp graph/POI.h)
