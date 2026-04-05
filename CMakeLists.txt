cmake_minimum_required(VERSION 4.1)
project(RinaLang)

set(CMAKE_CXX_STANDARD 20)

add_executable(RinaLang main.cpp
        runtime/exceptions.h
        runtime/entity.h
        compiler/ids.hpp
        compiler/ast.hpp
        compiler/types.hpp
        compiler/symbols.hpp)
