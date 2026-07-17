#include <language/entity/entity_base.hpp>
#include <language/entity/entity_id.hpp>
#include <vector>
#include <language/entity/component/i_requirement.h>
#include "language/entity/entity_components_init.h"
#include "language/exception/components_exceptions.h"

namespace rinalang::language::entity {

    entity_base::entity_base(std::vector<entity_component_init> inits) {
        if (inits.size() > 0) {
            for (auto& init : inits) {
                attach(init.key, std::move(init.holder));
            }
        }
        auto requirements = static_cast<i_requirement*>(this)->get_requirement();
        if (requirements.size() > 0) {
            for (auto& req : requirements) {
                if (!has(req)) {
                    throw new exception::required_not_attached_exception("Required component is not attached.");
                }
            }
        }
    }

    bool entity_base::operator==  (const entity_base& other) const {
        return other._id == _id;
    }

}
