cmake_minimum_required(VERSION 3.20)
project(RinaLang)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        RinaLang main.cpp
        public/class.hpp
        public/interface.hpp
        public/entity.hpp
        public/component.hpp
        public/exceptions.hpp
        private/class.cpp
        private/entity.cpp
        private/interface.cpp
)
