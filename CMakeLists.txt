cmake_minimum_required(VERSION 3.20)
project(RinaLang)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        RinaLang main.cpp
        public/runtime/exceptions.hpp
        public/runtime/entity.hpp
        public/compiler/lexer.hpp
        private/compiler/lexer.cpp
        public/compiler/file_read.hpp
        public/compiler/parser.hpp
        private/compiler/parser.cpp
        public/compiler/ast_printer_snippet.hpp
)
