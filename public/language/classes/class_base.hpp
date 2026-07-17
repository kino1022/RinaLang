#pragma once
#include <string>

namespace rinalang::language::classes {
  
  class i_implementable {};
  
  class class_base : public i_implementable {
    
  protected:
    
    virtual ~class_base() {}
    
  public:
    
    virtual std::string to_string () const noexcept;
    
    virtual int32_t get_hash_code () const noexcept;
    
  };
  
}
