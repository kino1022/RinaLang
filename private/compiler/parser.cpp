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

    token parser::parse_simple_expr_token() {
        const token& t = peek();
        if (t.kind == token_kind::IntLiteral || t.kind == token_kind::Identifier) {
            i_++;
            return t;
        }
        throw parse_error(t.pos, "expected simple expression (IntLiteral or Identifier)");
    }

    arg parser::parse_arg() {
        const token& t = peek();
        if (t.kind == token_kind::KwRef) {
            source_pos start = t.pos;
            i_++;
            place p = parse_place();
            arg a;
            a.kind = arg::kind_t::Ref;
            a.pos = p.pos;
            a.ref_place = std::move(p);
            return a;
        }
        
        token v = parse_simple_expr_token();
        arg a;
        a.kind = arg::kind_t::Value;
        a.pos = v.pos;
        a.value_token = std::move(v);
        return a;
    }
    
    
    call_expr parser::parse_call() {
        // call := Identifier '(' [args] ')'
        const token& name = expect(token_kind::Identifier, "identifier (callee)");
        source_pos start = name.pos;

        expect(token_kind::LParen, "'('");

        call_expr c;
        c.callee = name.lexeme;
        c.pos = start;

        if (!match(token_kind::RParen)) {
            // at least one arg
            c.args.push_back(parse_arg());
            while (match(token_kind::Comma)) {
                c.args.push_back(parse_arg());
            }
            expect(token_kind::RParen, "')'");
        }

        return c;
    }
}
