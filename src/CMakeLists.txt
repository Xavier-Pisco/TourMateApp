cmake_minimum_required(VERSION 3.16)
project(src)

set(CMAKE_CXX_STANDARD 14)

add_executable(src main.cpp lib/GraphViewer/graphviewer.cpp lib/GraphViewer/graphviewer.h lib/GraphViewer/connection.cpp lib/GraphViewer/connection.h graph/GraphClasses.cpp graph/GraphClasses.h application/Application.cpp application/Application.h application/input/Converter.cpp application/input/Converter.h application/input/UserInput.cpp application/input/UserInput.h graph/Position.h application/User.cpp application/User.h graph/algorithms/dijkstra.h application/RouteMaker.cpp application/RouteMaker.h graph/POI.cpp graph/POI.h application/Menu.cpp application/Menu.h application/Drawer.cpp application/Drawer.h graph/viewGraph.h application/exceptions.h)
