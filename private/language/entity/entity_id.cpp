#include <cstdint>
#include <language/entity/entity_id.hpp>

namespace rinalang::language::entity {
  
  int32_t entity_id::_globalId = 0;

  entity_id::entity_id(const bool is_valid) {
    _id = is_valid? _globalId++ : 0;
  }

  bool entity_id::is_valid() const {
    return _id != 0;
  }

  bool entity_id::operator==(const entity_id& rhs) const {
    return rhs._id == _id;
  }

}
