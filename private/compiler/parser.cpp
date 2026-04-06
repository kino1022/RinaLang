#include "../../public/compiler/parser.hpp"

namespace crl {
    const token& parser::peek(size_t lookahead) const {
        size_t j = i_ + lookahead;
        if (j >= toks_.size()) {
            return toks_.back();
        }
        return toks_[j];
    }

    bool parser::is_at_end() const {
        return peek().kind == token_kind::Eof;
    }

    bool parser::match(token_kind k) {
        if (peek().kind == k) {
            i_++;
            return true;
        }
        return false;
    }

    const token& parser::expect(token_kind k, const char* what) {
        if (peek().kind != k) {
            const token& t = peek();
            throw parse_error(t.pos, std::string("expected") + what + ", but got [" + t.lexeme + "]");
        }
        const token& t = peek();
        i_++;
        return t;
    }

    place parser::parse_place() {
        const token& first = expect(token_kind::Identifier, "identifier(start of place)");
        
        place p;
        p.root = first.lexeme;;
        p.pos = first.pos;
        while (match(token_kind::Dot)) {
            const token& f = expect(token_kind::Identifier, "identifer(field name)");
            p.fields.push_back(f.lexeme);
        }
        return p;
    }

    arg parser::parse_arg() {
        const token& t = peek();
        if (t.kind == token_kind::KwRef) {
            source_pos start = t.pos;
            i_++;
            place p = parse_place();
            arg a;
            a.kind = arg::kind_t::Ref;
            a.pos = start;
            a.ref_place = std::move(p);
            return a;
        }
        
        arg a;
        a.kind = arg::kind_t::Value;
        a.pos = peek().pos;
        a.value = parse_expr();
        return a;
    }
    
    std::vector<arg> parser::parse_args_list_until_rparen() {
    std::vector<arg> args;
    if (match(token_kind::RParen)) return args; // empty ()

    args.push_back(parse_arg());
    while (match(token_kind::Comma)) {
        args.push_back(parse_arg());
    }
    expect(token_kind::RParen, "')'");
    return args;
}

// ---- expressions ----
expr_ptr parser::parse_expr() {
    // MVP: no binary operators yet, so expr == postfix
    return parse_postfix();
}

expr_ptr parser::parse_primary() {
    const token& t = peek();

    if (t.kind == token_kind::IntLiteral) {
        token tok = t; i_++;
        auto e = std::make_unique<expr>();
        e->pos = tok.pos;
        e->node = expr_int{tok};
        return e;
    }

    if (t.kind == token_kind::Identifier) {
        token tok = t; i_++;
        auto e = std::make_unique<expr>();
        e->pos = tok.pos;
        e->node = expr_var{tok};
        return e;
    }

    if (match(token_kind::LParen)) {
        expr_ptr inner = parse_expr();
        expect(token_kind::RParen, "')'");
        return inner;
    }

    throw parse_error(t.pos, "expected primary expression");
}

expr_ptr parser::parse_postfix() {
    expr_ptr base = parse_primary();

    while (true) {
        // field access
        if (match(token_kind::Dot)) {
            const token& f = expect(token_kind::Identifier, "identifier (field name)");
            auto e = std::make_unique<expr>();
            e->pos = base->pos;
            e->node = expr_field{std::move(base), token(f)};
            base = std::move(e);
            continue;
        }

        // call: base(args)  ※ calleeがIdentifierであることを要求（MVP���
        if (match(token_kind::LParen)) {
            // base must be expr_var (Identifier) in MVP
            if (!std::holds_alternative<expr_var>(base->node)) {
                throw parse_error(base->pos, "only identifier calls are supported in MVP (e.g. foo(...))");
            }
            token callee = std::get<expr_var>(base->node).tok;

            std::vector<arg> args = parse_args_list_until_rparen();

            auto e = std::make_unique<expr>();
            e->pos = callee.pos;
            e->node = expr_call{callee, std::move(args)};
            base = std::move(e);
            continue;
        }

        break;
    }

    return base;
}

expr_ptr parser::parse_call_expr() {
    // For --parse-call: parse an expr and require it ends at Eof
    expr_ptr e = parse_expr();
    expect(token_kind::Eof, "end of file");
        return e;// copy expr (contains unique_ptr, so this won't compile)
}
}
