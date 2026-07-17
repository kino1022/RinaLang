//
// Created by 藤田優太 on 2026/07/17.
//

#include "language/entity/component/component_base.h"

namespace rinalang::language::entity::component {

    void component_base::attach(const std::shared_ptr<entity_base>& owner) {
        _owner = owner;
        on_attached();
    }

    void component_base::detach() {
        _owner.reset();
        _owner = nullptr;
        on_detached();
    }


}
