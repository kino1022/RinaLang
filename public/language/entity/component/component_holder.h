//
// Created by 藤田優太 on 2026/07/17.
//

#pragma once

#include <concepts>

#ifndef RINALANG_COMPONENT_HOLDER_H
#define RINALANG_COMPONENT_HOLDER_H

#endif //RINALANG_COMPONENT_HOLDER_H

namespace rinalang::language::entity::component {

    struct i_component_holder {

        virtual ~i_component_holder() = default;

        virtual component_base  *ptr () =0;

        virtual const component_base  *ptr () const =0;

    };

    template<typename T>
    requires std::derived_from<T, component_base>
    struct component_holder final : i_component_holder  {

        template <class... Args>
        explicit component_holder(Args &&...args) : value(std::forward<Args>(args)...) {}

        component_base *ptr() override { return &value; }
        const component_base *ptr() const override { return &value; }

        T value;
    };
}
