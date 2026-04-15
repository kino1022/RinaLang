#include "../public/entity.hpp"

#include "../public/exceptions.hpp"

namespace rinalang {
    
    entity::entity_base::entity_base(const std::vector<entity_component_init>& init_components) {
        if (init_components.size() > 0) {
            for (auto comp : init_components) {
                attach(static_cast<i_component_holder<comp.key>>(comp.holder));
            }
        }
        if (required_components_.size() > 0) {
            for (auto req : required_components_) {
                if (!has<req>) {
                    throw exceptions::required_not_attached_exception("Required component not attached: " + std::string(typeid(req).name()));
                }
            }
        }
    }
}
