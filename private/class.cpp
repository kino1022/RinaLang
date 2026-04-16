#include "../public/class.hpp"

namespace rinalang {
    
    bool class_base::operator==(const class_base& other) const {
        return this == &other;
    }
    
    bool class_base::operator!=(const class_base& other) const {
        return !(*this == other);
    }

    void class_base::on_start() const {
        for (const auto& met : start_events) {
            using func_t = void(*)(const class_base&);
            auto func = reinterpret_cast<func_t>(met);
            func(*this);
        }
    }

    void class_base::on_update(const float delta_time) const {
        for (const auto& met : update_events) {
            using func_t = void(*)(const class_base&, const float);
            auto func = reinterpret_cast<func_t>(met);
            func(*this, delta_time);
        }
    }
}
