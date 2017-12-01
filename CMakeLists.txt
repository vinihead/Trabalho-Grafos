cmake_minimum_required(VERSION 3.8)
project(TrabalhoGrafos)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Aresta.cpp
        Aresta.h
        Grafo.cpp
        Grafo.h
        main.cpp
        Vertice.cpp
        Vertice.h
        Cadeia.cpp
        Cadeia.h)

add_executable(TrabalhoGrafos ${SOURCE_FILES})
