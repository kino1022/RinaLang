#pragma once

#include <typeindex>
#include <unordered_map>
#include <vector>
#include "class.hpp"

namespace rinalang::entity {
    
    class component_base;
    
    struct i_component_holder {
        
        virtual ~i_component_holder() = default;
        
        virtual component_base* ptr() =0;
        
        virtual const component_base* ptr() const =0;
        
    };
    
    template<class T>
    requires std::derived_from<T, component_base>
    struct component_holder final : i_component_holder {
        template <class... Args>
        explicit component_holder(Args&&... args) : value(std::forward<Args>(args)...) {}

        component_base* ptr() override { return &value; }
        const component_base* ptr() const override { return &value; }

        T value;
    };
    
    class entity_base : public class_base{
        
    public:
        entity_base() = default;
        virtual ~entity_base() override = default;
        
        virtual std::unique_ptr<entity_base> clone() const =0;
        
        entity_base(const entity_base&) = delete;
        entity_base& operator=(const entity_base&) = delete;
        entity_base(entity_base&&) = default;
        entity_base& operator=(entity_base&&) = default;
        
        template<class T, class... Args>
        requires std::derived_from<T, component_base>
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
        
        template<class T>
        requires std::derived_from<T, component_base>
        bool remove () {
            const std::type_index key(typeid(T));
            if (components.contains(key)) {
                // requiredなコンポーネントが消えないようにするために
                for (auto comp : required_components_) {
                    if (comp == key) {
                        return false;
                    }
                }
                components.erase(key);
                return true;
            }
            return false;
        }
        
        template<class T, class... Args>
        bool has () const {
            const std::type_index key(typeid(T));
            return components.contains(key);
        }
        
    protected:
        std::vector<std::type_index> required_components_;
        
    private:
        std::unordered_map<std::type_index, std::unique_ptr<i_component_holder>> components;
    };
}