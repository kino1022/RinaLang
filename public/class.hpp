#pragma once
#include <vector>
#include <memory>
#include <functional>

#include "interface.hpp"

namespace rinalang {
    
    class implementable;
    
    /// @brief Rinalangのclass型の既定クラス
    class class_base : public implementable {
    public:
    
        class_base () = default;

        virtual ~class_base() = default;
    
        virtual bool operator== (const class_base& other) const;

        virtual bool operator!= (const class_base& other) const;

    };
}
