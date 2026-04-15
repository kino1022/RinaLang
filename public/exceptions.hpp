#pragma once
#include <stdexcept>

namespace rinalang::exceptions {
    
    struct runtime_exception : std::runtime_error {
        using std::runtime_error::runtime_error;
    };
    
    struct component_notfound_exception : runtime_exception {
        using runtime_exception::runtime_exception;
    };
    
    struct component_same_type_exception : runtime_exception {
        using runtime_exception::runtime_exception;
    };
    
    struct required_not_attached_exception : runtime_exception {
        using runtime_exception::runtime_exception;
    };
    
}
