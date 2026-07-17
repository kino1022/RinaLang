#pragma once
#include <cstdint>

namespace rinalang::language::entity{
  
  class entity_id {
  private:
    
    static int32_t _globalId;
    
    int32_t _id;
    
  public:
    
    entity_id(const bool is_valid = true);

    bool is_valid() const;
    
    bool operator==(const entity_id& rhs) const;
    
  };
  
}
