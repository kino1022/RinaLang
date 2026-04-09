#include "../public/class.hpp"

namespace rinalang {
    
    bool class_base::operator==(const class_base& other) const {
        return this == &other;
    }
    
    bool class_base::operator!=(const class_base& other) const {
        return !(*this == other);
    }
    
}
