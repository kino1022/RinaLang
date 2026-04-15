#pragma once
#include <concepts>
#include <typeindex>

namespace rinalang {
    
    class interface_base {
        
    };
    
    class implementable {
    public:
        template<class T>
        requires std::derived_from<T, interface_base>
        bool implements_interface () const {
            return static_cast<T>(this) != nullptr;
        }
    };
    
}

