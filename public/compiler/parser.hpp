#pragma once
#include <stdexcept>

#include "lexer.hpp"

namespace crl {
    struct parse_error : public std::runtime_error {
        source_pos pos;
        parse_error(const source_pos& pos, const std::string& msg) : runtime_error(msg), pos(pos) {}
    };
    
    struct place {
        std::string root;
        std::vector<std::string> fields;
        source_pos pos;
    };
    
    struct arg {
        enum class kind_t {
            Value, 
            Ref
        }kind;
        source_pos pos;
        std::optional<token> value_token;
        std::optional<place> ref_place;
    };
    
    struct call_expr {
        std::string callee;
        std::vector<arg> args;
        source_pos pos;
    };
    
    class parser {
    public:
        explicit parser(const std::vector<token>& tokens) : toks_(tokens) {}
        
        place parse_place();
        
        call_expr parse_call();
    private:
        const std::vector<token>& toks_;
        size_t i_ = 0;
        
        const token& peek(size_t lookahead = 0) const;
        bool match (token_kind k);
        const token&  expect(token_kind k, const char* what);
        bool is_at_end() const;
        
        arg parse_arg();
        
        token parse_simple_expr_token();
        
    };
}
