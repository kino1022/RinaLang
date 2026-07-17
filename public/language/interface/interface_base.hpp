#pragma once

namespace rinalang::language::interface {

    class i_implementable {
    public:
        template <typename T>
        bool implementation() const noexcept {
            return static_cast<const T*>(this)->is_implemented();
        }
    };

    class interface_base : public i_implementable {

    public:
        interface_base() = delete;
        ~interface_base() = delete;
        
    };
}