#pragma once

#include <typeindex>
#include <unordered_map>

#include "exceptions.hpp"

namespace rinalang::entity {
    
    struct i_component_holder {
        
        virtual ~i_component_holder() = default;
        
        virtual void* ptr() =0;
        
        virtual const void* ptr() const =0;
        
    };
    
    template<class T>
    struct component_holder final : i_component_holder {
        template <class... Args>
        explicit component_holder(Args&&... args) : value(std::forward<Args>(args)...) {}

        void* ptr() override { return &value; }
        const void* ptr() const override { return &value; }

        T value;
    };
    
    class entity_base {
        
    public:
        entity_base() = default;
        virtual ~entity_base() = default;
        
        entity_base(const entity_base&) = delete;
        entity_base& operator=(const entity_base&) = delete;
        entity_base(entity_base&&) = default;
        entity_base& operator=(entity_base&&) = default;
        
        template<class T, class... Args>
        T& add (Args&&... args) {
            const std::type_index key(typeid(T));
            if (components.contains(key)) {
                components.erase(key);
            }
            auto holder = std::make_unique<component_holder<T>>(std::forward<Args>(args)...);
            T& ref = holder->value;
            components.emplace(key, std::move(holder));
            return ref;
        }
        
        template<class T, class... Args>
        bool has () const {
            const std::type_index key(typeid(T));
            return components.contains(key);
        }
        
    private:
        std::unordered_map<std::type_index, std::unique_ptr<i_component_holder>> components;
    };
}