//
// Created by 藤田優太 on 2026/07/17.
//
#pragma once

#ifndef RINALANG_COMPONENT_BASE_H
#define RINALANG_COMPONENT_BASE_H
#include "i_requirement.h"


#endif //RINALANG_COMPONENT_BASE_H

namespace rinalang::language::entity {
    class entity_base;
}

namespace rinalang::language::entity::component {

    class i_implementable {};

    class class_base : public i_implementable {};

    class component_base : public class_base, public i_requirement {

    private:

        std::shared_ptr<entity_base> _owner;

    public:

        void attach (const std::shared_ptr<entity_base>& owner);

        void detach ();

        virtual void on_attached () =0;

        virtual void on_detached () =0;

    };

}

