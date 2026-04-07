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
        return parse_equality();
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

        if (t.kind == token_kind::StringLiteral) {
            token tok = t;
            i_++;
            auto e = std::make_unique<expr>();
            e->pos = tok.pos;
            e->node = expr_string{tok};
            return e;
        }
            
        if (t.kind == token_kind::KwTrue || t.kind == token_kind::KwFalse) {
            token tok = t; 
            i_++;
            auto e = std::make_unique<expr>();
            e->pos = tok.pos;
            e->node = expr_bool{ tok.kind == token_kind::KwTrue, tok };
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
                std::vector<arg> args = parse_args_list_until_rparen();
                auto e = std::make_unique<expr>();
                e->pos = base->pos;
                e->node = expr_call{ std::move(base), std::move(args) };
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

    expr_ptr parser::parse_equality() {
        expr_ptr lhs = parse_comparison();
        while (true) {
            token_kind k = peek().kind;
            if (k != token_kind::EqualEqual && k != token_kind::NotEqual) break;
            
            token op = peek();
            i_++;
            
            expr_ptr rhs = parse_add();
            auto e = std::make_unique<expr>();
            e->pos = lhs->pos;
            e->node = expr_binary{
                std::move(op),
                std::move(lhs),
                std::move(rhs)
            };
            lhs = std::move(e);
        }
        return lhs;
    }
    
    expr_ptr parser::parse_add() {
        expr_ptr lhs = parse_unary();
        while (true) {
            token_kind k = peek().kind;
            if (k != token_kind::Plus && k != token_kind::Minus) break;
            
            token op = peek();
            i_++;
            
            expr_ptr rhs = parse_unary();
            
            auto e = std::make_unique<expr>();
            e->pos = lhs->pos;
            e->node = expr_binary{
                std::move(op),
                std::move(lhs),
                std::move(rhs)
            };
            lhs = std::move(e);
        }
        return lhs;
    }

    expr_ptr parser::parse_unary() {
        if (peek().kind == token_kind::Minus) {
            token op = peek();
            i_++; // consume '-'

            expr_ptr rhs = parse_unary();

            auto e = std::make_unique<expr>();
            e->pos = op.pos;
            e->node = expr_unary{ std::move(op), std::move(rhs) };
            return e;
        }

        return parse_postfix();
    }

    expr_ptr parser::parse_comparison() {
        expr_ptr lhs = parse_add();
        while (true) {
            token_kind k = peek().kind;
            if (k != token_kind::Less &&
                k != token_kind::Greater &&
                k != token_kind::LessEqual &&
                k != token_kind::GreaterEqual ) {
                break;
            }
            
            token op = peek();
            i_++;
            
            expr_ptr rhs = parse_add();
            
            auto e = std::make_unique<expr>();
            e->pos = lhs->pos;
            e->node = expr_binary{std::move(op), std::move(lhs), std::move(rhs)};
            lhs = std::move(e);
        }
        return lhs;
    }

    block parser::parse_block() {
        const token& lb = expect(token_kind::LBrace, "'{'");
        block b;
        b.pos = lb.pos;
        while (!match(token_kind::RBrace)) {
            if (is_at_end()) {
                throw parse_error(peek().pos, "expected '}' before end of file");
            }
            b.stmts.push_back(parse_stmt());
        }
        return b;
    }

    block parser::parse_block_eof() {
        block b = parse_block();
        expect(token_kind::Eof, "end of file");
        return b;
    }
    
    stmt_ptr parser::parse_stmt() {
        const token& t = peek();

        // return
        if (t.kind == token_kind::KwReturn) {
            token ret = t; i_++;

            auto s = std::make_unique<stmt>();
            s->pos = ret.pos;

            // return;
            if (match(token_kind::SemiColon)) {
                s->node = stmt_return{ std::nullopt };
                return s;
            }

            // return expr;
            expr_ptr v = parse_expr();
            expect(token_kind::SemiColon, "';'");
            stmt_return r;
            r.value = std::move(v);
            s->node = std::move(r);
            return s;
        }

        // let [mut] name = expr;
        if (t.kind == token_kind::KwLet) {
            token letTok = t; i_++;

            bool is_mut = false;
            if (match(token_kind::KwMut)) is_mut = true;

            token name = expect(token_kind::Identifier, "identifier (let name)");
            expect(token_kind::Assign, "'='");
            expr_ptr init = parse_expr();
            expect(token_kind::SemiColon, "';'");

            auto s = std::make_unique<stmt>();
            s->pos = letTok.pos;
            s->node = stmt_let{ is_mut, name, std::move(init) };
            return s;
        }

        // assignment: place '=' expr ';'
        // 先読み：Identifier '.' ... '=' なら assignment とみなす
        // NOTE: ここは “まずplaceを試す” 実装にします（失敗したら expr文へ）。
        if (t.kind == token_kind::Identifier) {
            // place をパースしてみる（成功したとしても '=' が無ければ巻き戻して expr 文へ）
            size_t save = i_;
            try {
                place lhs = parse_place();
                if (match(token_kind::Assign)) {
                    expr_ptr rhs = parse_expr();
                    expect(token_kind::SemiColon, "';'");
                    auto s = std::make_unique<stmt>();
                    s->pos = lhs.pos;
                    s->node = stmt_assign{ std::move(lhs), std::move(rhs) };
                    return s;
                }
            } catch (...) {
                // ignore and fallthrough to expr stmt
            }
            i_ = save; // rollback if not assignment
        }

        // expr;
        expr_ptr e = parse_expr();
        expect(token_kind::SemiColon, "';'");
        auto s = std::make_unique<stmt>();
        s->pos = e->pos;
        s->node = stmt_expr{ std::move(e) };
        return s;
    }

    stmt_ptr parser::parse_stmt_eof() {
        stmt_ptr s = parse_stmt();
        expect(token_kind::Eof, "end of file");
        return s;
    }

    type_name parser::parse_type_name() {
        token t = expect(token_kind::Identifier, "type name (identifier)");
        return type_name{t};
    }

    std::vector<param> parser::parse_param_list_until_rparen() {
        std::vector<param> ps;
        if (match(token_kind::RParen)) {
            return ps;
        }
        while (true) {
            token name = expect(token_kind::Identifier, "parameter name");
            
            param p;
            p.name = name;
            
            if (match (token_kind::Colon)) {
                p.ty = parse_type_name();
            }
            
            ps.push_back(std::move(p));
            
            if (match(token_kind::Comma)) {
                continue;
            }
            expect(token_kind::RParen, "')'");
            break;
        }
        return ps;
    }

    std::optional<type_name> parser::try_parse_ret_type() {
        if (!match(token_kind::Colon)) {
            return std::nullopt;
        }
        return parse_type_name();
    }

    fn_item parser::parse_fn() {
        const token& kw = expect(token_kind::KwFn, "fn");
        token name = expect(token_kind::Identifier, "function name");
        
        expect(token_kind::LParen, "'('");
        std::vector<param> params = parse_param_list_until_rparen();
        
        std::optional<type_name> ret = try_parse_ret_type();
        
        block body = parse_block();
        
        fn_item f;
        f.pos = kw.pos;
        f.name = name;
        f.params = std::move(params);
        f.ret_type = std::move(ret);
        f.body = std::move(body);
        return f;
    }

    fn_item parser::parse_fn_eof() {
        fn_item f = parse_fn();
        expect(token_kind::Eof, "end of file");
        return f;
    }
}
