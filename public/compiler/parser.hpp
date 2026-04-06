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
    
    struct expr;
    using expr_ptr = std::unique_ptr<expr>;

    struct expr_int { token tok; };        // IntLiteral
    struct expr_var { token tok; };        // Identifier
    struct expr_field { expr_ptr base; token field; }; // base.field
    struct expr_call;
    
    struct arg {
        enum class kind_t {
            Value, 
            Ref
        }kind;
        source_pos pos;
        expr_ptr value;
        std::optional<place> ref_place;
    };
    
    struct expr_call {
        token callee;
        std::vector<arg> args;
    };
    
    struct expr {
        source_pos pos;
        std::variant<expr_int,expr_var, expr_field, expr_call> node;
    };
    
    class parser {
    public:
        explicit parser(const std::vector<token>& tokens) : toks_(tokens) {}
        
        place parse_place();
        
        expr_ptr parse_call_expr();
    private:
        const std::vector<token>& toks_;
        size_t i_ = 0;
        
        const token& peek(size_t lookahead = 0) const;
        bool match (token_kind k);
        const token&  expect(token_kind k, const char* what);
        
        expr_ptr parse_expr();
        expr_ptr parse_postfix();
        expr_ptr parse_primary();
        
        arg parse_arg();
        std::vector<arg> parse_args_list_until_rparen();
        
        bool is_at_end() const;
    };
}
