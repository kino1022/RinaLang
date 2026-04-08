#pragma once
#include <stdexcept>
#include <unordered_map>

#include "lexer.hpp"
#include "parser.hpp"

namespace crl {
    
    struct type_error : std::runtime_error {
        source_pos pos;
        type_error(source_pos p, const std::string& msg) : runtime_error(msg), pos(p) {}
    };
    
    struct fn_sig {
        std::vector<type_ptr> params;
        type_ptr ret;
    };
    
    using type_env = std::unordered_map<std::string, type_ptr>;
    using fn_env = std::unordered_map<std::string, fn_sig>;
    
    bool type_equals (const type& a, const type& b);
    
    std::string type_to_string (const type& t);
    
    void typecheck_fn(const fn_item& f, const fn_env& fns);
    
}
