cmake_minimum_required(VERSION 3.20)
project(RinaLang)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        
    RinaLang
        main.cpp

    public/language/interface/interface_base.hpp
    public/language/classes/class_base.hpp
    public/language/entity/entity_id.hpp
    private/language/entity/entity_id.cpp
    public/language/entity/entity_base.hpp
    private/language/entity/entity_base.cpp
    public/language/entity/entity_base.hpp
        private/language/entity/entity_base.cpp
        private/language/entity/entity_id.cpp
        public/language/entity/component/component_base.h
        public/language/entity/component/i_requirement.h
        public/language/entity/component/i_requirement.h
        public/language/entity/component/component_holder.h
        public/language/entity/entity_components_init.h
        public/language/exception/components_exceptions.h
        private/language/entity/component/component_base.cpp
)

target_include_directories(
    RinaLang
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
)
