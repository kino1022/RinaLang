cmake_minimum_required(VERSION 4.1)
project(RinaLang)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        RinaLang main.cpp
        public/runtime/exceptions.hpp
        public/runtime/entity.hpp
        public/compiler/lexer.hpp
        private/compiler/lexer.cpp
        private/runtime/exceptions.cpp
)
