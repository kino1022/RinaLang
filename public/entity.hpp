#pragma once

#include <typeindex>
#include <unordered_map>
#include <vector>
#include "class.hpp"
#include "component.hpp"

namespace rinalang
{

    struct i_component_holder
    {

        virtual ~i_component_holder() = default;

        virtual component_base *ptr() = 0;

        virtual const component_base *ptr() const = 0;
    };

    template <class T>
        requires std::derived_from<T, component_base>
    struct component_holder final : i_component_holder
    {
        template <class... Args>
        explicit component_holder(Args &&...args) : value(std::forward<Args>(args)...) {}

        component_base *ptr() override { return &value; }
        const component_base *ptr() const override { return &value; }

        T value;
    };

    /// @brief entityの初期化の際に渡すコンポーネントの初期化のための構造体
    struct entity_component_init
    {
        std::type_index key;
        std::unique_ptr<i_component_holder> holder;

        template <class T, class... Args>
            requires std::derived_from<T, component_base>
        static entity_component_init make(Args &&...args)
        {
            auto h = std::make_unique<component_holder<T>>(std::forward<Args>(args)...);
            return {
                std::type_index(typeid(T)),
                std::move(h)};
        }
    };

    class entity_base : public class_base
    {

    public:
        entity_base(std::vector<entity_component_init> init_components);
        virtual ~entity_base() override = default;

        virtual std::unique_ptr<entity_base> clone() const = 0;

        template <class T, class... Args>
            requires std::derived_from<T, component_base>
        T &add(Args &&...args)
        {
            const std::type_index key(typeid(T));
            if (components.contains(key))
            {
                components.erase(key);
            }
            auto holder = std::make_unique<component_holder<T>>(std::forward<Args>(args)...);
            T &ref = holder->value;
            components.emplace(key, std::move(holder));
            return ref;
        }

        template <class T>
            requires std::derived_from<T, component_base>
        bool remove()
        {
            const std::type_index key(typeid(T));
            if (components.contains(key))
            {
                // requiredなコンポーネントが消えないようにするために
                for (auto comp : required_components_)
                {
                    if (comp == key)
                    {
                        return false;
                    }
                }
                components.erase(key);
                return true;
            }
            return false;
        }

        template <class T>
            requires std::derived_from<T, component_base>
        bool has() const
        {
            const std::type_index key(typeid(T));
            return components.contains(key);
        }

        bool has(std::type_index key) const
        {
            return components.contains(key);
        }

        template <class T>
            requires std::derived_from<T, component_base>
        T &get_ref() const
        {
            const std::type_index key(typeid(T));
            if (components.contains(key))
            {
                return components.at(key);
            }
            return nullptr;
        }

        template <class T>
            requires std::derived_from<T, component_base>
        T get() const
        {
            const std::type_index key(typeid(T));
            if (components.contains(key))
            {
                return components.at(key);
            }
            return nullptr;
        }

    protected:
        std::vector<std::type_index> required_components_;

        void attach(std::type_index key, std::unique_ptr<i_component_holder> holder){
            components.insert_or_assign(key, std::move(holder));
        }

        template <class T>
            requires std::derived_from<T, component_base>
        void attach(std::unique_ptr<component_holder<T>> holder){
            const std::type_index key(typeid(T));
            components.insert_or_assign(key, std::move(holder));
        }

    private:
        std::unordered_map<std::type_index, std::unique_ptr<i_component_holder>> components;
    };
}