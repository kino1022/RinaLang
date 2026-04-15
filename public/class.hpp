#pragma once
#include <memory>

#include "interface.hpp"

namespace rinalang {
    
    class implementable;
    
    /// @brief Rinalangのclass型の既定クラス
    class class_base : public implementable {
    public:
    
        class_base () = default;
        virtual ~class_base() () = default;
    
        virtual std::unique_ptr<class_base> clone () = 0;
    
        virtual bool operator== (const class_base& other) const;
        virtual bool operator!= (const class_base& other) const;
        
        template<class T, class... Args>
        bool implements() {
            return static_cast<Args>(this);
        }
    };
}
