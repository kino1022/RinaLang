//
// Created by 藤田優太 on 2026/07/17.
//

#pragma once

#ifndef RINALANG_ENTITY_COMPONENTS_INIT_H
#define RINALANG_ENTITY_COMPONENTS_INIT_H
#include <memory>
#include <typeindex>

#include "component/component_base.h"
#include "component/component_holder.h"

#endif //RINALANG_ENTITY_COMPONENTS_INIT_H

namespace rinalang::language::entity {
    /// @brief entityの初期化の際に渡すコンポーネントの初期化のための構造体
    struct entity_component_init {

        std::type_index key;

        std::unique_ptr<component::i_component_holder> holder;

        template <class T, class... Args>
            requires std::derived_from<T, component::component_base>
        static entity_component_init make(Args &&...args) {
            auto h = std::make_unique<component::component_holder<T>>(std::forward<Args>(args)...);
            return {
                std::type_index(typeid(T)),
                std::move(h)};
        }
    };
}
