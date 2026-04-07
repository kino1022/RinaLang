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
    struct expr_string {token tok;};
    struct expr_bool {bool value; token tok;};
    
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
        expr_ptr callee;
        std::vector<arg> args;
    };
    
    struct expr_binary {
        token op;
        expr_ptr lhs;
        expr_ptr rhs;
    };
    
    struct expr_unary {
        token op;
        expr_ptr rhs;
    };
    
    struct expr {
        source_pos pos;
        std::variant<
            expr_int,expr_var, 
            expr_field,
            expr_call,
            expr_string,
            expr_bool,
            expr_binary,
            expr_unary
        > node;
    };
    
    struct stmt;
    using stmt_ptr = std::unique_ptr<stmt>;
    
    struct type_name {
        token name;
    };
    
    struct type;
    using type_ptr = std::unique_ptr<type>;
    
    struct type_named { token name; };
    struct type_ref { type_ptr base; };
    
    struct type {
        source_pos pos;
        std::variant<type_named, type_ref> node;
    };
    
    struct stmt_let {
        bool is_mut;
        token name;
        std::optional<type_ptr> ty;
        expr_ptr init;
    };
    
    struct stmt_assign {
        place lhs;
        expr_ptr rhs;
    };
    
    struct stmt_return {
        std::optional<expr_ptr> value;
    };
    
    struct stmt_expr {
        expr_ptr value;
    };
    
    struct stmt {
        source_pos pos;
        std::variant<
            stmt_let,
            stmt_assign,
            stmt_return,
            stmt_expr
            > node;
    };
    
    struct block {
        source_pos pos;
        std::vector<stmt_ptr> stmts;
    };
    
    struct param {
        token name;
        std::optional<type_ptr> ty;
    };
    
    struct fn_item {
        token name;
        std::vector<param> params;
        std::optional<type_ptr> ret_type;
        block body;
        source_pos pos;
    };
    
    
    class parser {
    public:
        explicit parser(const std::vector<token>& tokens) : toks_(tokens) {}
        
        place parse_place();
        
        expr_ptr parse_call_expr();
        
        stmt_ptr parse_stmt_eof();
        block parse_block_eof();
        
        fn_item parse_fn_eof();
        
    private:
        const std::vector<token>& toks_;
        size_t i_ = 0;
        
        const token& peek(size_t lookahead = 0) const;
        bool match (token_kind k);
        const token&  expect(token_kind k, const char* what);
        
        expr_ptr parse_expr();
        expr_ptr parse_postfix();
        expr_ptr parse_primary();
        
        expr_ptr parse_equality();
        expr_ptr parse_add();
        expr_ptr parse_unary();
        
        expr_ptr parse_comparison();
        
        type_ptr parse_type();
        type_ptr parse_type_named();
        
        // parser private:
        stmt_ptr parse_stmt();
        block parse_block();
        
        fn_item parse_fn();
        std::vector<param> parse_param_list_until_rparen();
        std::optional<type_name> try_parse_ret_type();
        type_name parse_type_name();
        
        arg parse_arg();
        std::vector<arg> parse_args_list_until_rparen();
        
        bool is_at_end() const;
    };
}
