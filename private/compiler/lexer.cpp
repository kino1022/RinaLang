
#include "../../public/compiler/lexer.hpp"

#include <unordered_map>

std::vector<crl::token> crl::lexer::lex_all() {
    std::vector<token> out;
    while (true) {
        token t = next();
        out.push_back(t);
        if (t.kind == token_kind::Eof) {
            break;
        }
    }
    return out;
}

crl::token crl::lexer::next() {
    skip_comment_and_space();
    source_pos start = pos_;
    char c = peek();
    if (c == '\0') {
        return make(token_kind::Eof, "", start);
    }
    if (is_ident_start(c)) {
        return lex_ident_or_keyword();
    }
    if (std::isdigit(static_cast<unsigned char>(c))) {
        return lex_int();
    }
    if (c == '"'){
        return lex_string();
    }
    if (c == '-' && peek() == '>'){
        advance();
        advance();
        return make (token_kind::Arrow, "->", start);
    }
    if (c == '=' && peek() == '=') {
        advance();
        advance();
        return make(token_kind::EqualEqual, "==", start);
    }
    if (c == '!' && peek() == '=') {
        advance();
        advance();
        return make(token_kind::NotEqual, "!=", start);
    }
    
    switch (c) {
        case '{' : advance (); return make (token_kind::LBrace, "{", start);
        case '}' : advance (); return make (token_kind::RBrace, "}", start);
        case '(' : advance (); return make (token_kind::LParen, "(", start);
        case ')' : advance (); return make (token_kind::RParen, ")", start);
        case ',' : advance (); return make (token_kind::Comma, ",", start);
        case '.' : advance (); return make (token_kind::Dot, ".", start);
        case ':' : advance (); return make (token_kind::Colon, ":", start);
        case ';' : advance (); return make (token_kind::SemiColon, ";", start);
        case '+' : advance(); return make (token_kind::Plus, "+", start);
        case '-' : advance (); return make (token_kind::Minus, "-", start);
        case '=' : advance (); return make (token_kind::Assign, "=", start);
    default:
        throw lex_error(start, std::string("Unexpected character: '") + c + "'");
    }
}

char crl::lexer::peek(size_t lookahead) const {
    size_t j = i_ + lookahead;
    if (j >= input_.size()) {
        return '\0';
    }
    return input_[j];
}

void crl::lexer::advance() {
    char c = peek();
    if (c == '\0') {
        return;
    }
    if (c == '\n') {
        pos_.column = 1;
        pos_.line++;
    }
    else {
        pos_.column++;
    }
}

crl::token crl::lexer::make(token_kind kind, std::string_view lexeme, source_pos pos) {
    return crl::token{
        kind, 
        std::string(lexeme), 
        pos
    };
}

bool crl::lexer::is_ident_start(char c) {
    unsigned char uc = static_cast<unsigned char>(c);
    return std::isalpha(uc) || c == '_';
}

bool crl::lexer::is_ident_count(char c) {
    unsigned char uc = static_cast<unsigned char>(c);
    return std::isalnum(uc) || c == '_';
}

void crl::lexer::skip_comment_and_space() {
    while (true) {
        char c = peek();
        if (c == '' || c == '\t' || c == '\n' || c == '\r') {
            advance();
            continue;
        }
        if (c == '/' && peek(1) == '/' ) {
            while (peek() != '\0' && peek() != '\n') {
                advance();
            }
            continue;
        }
        break;
    }
}

crl::token crl::lexer::lex_int() {
    source_pos start = pos_;
    size_t begin = i_;
    while (std::isdigit(static_cast<unsigned char>(peek()))) {
        advance();
    }
    std::string lex(input_.substr(begin, i_ - begin));
    return make (token_kind::IntLiteral, move(lex), start);
}

crl::token crl::lexer::lex_float() {
    source_pos start = pos_;
    size_t begin = i_;
    while (std::isdigit(static_cast<unsigned char>(peek()))) {
        advance();
    }
    std::string lex(input_.substr(begin, i_ - begin));
    return make (token_kind::FloatLiteral, move(lex), start);
}

crl::token crl::lexer::lex_string() {
    source_pos start = pos_;
    advance();
    std::string value;
    while (true) {
        char c = peek();
        if (c == '\0' || c == '\n') {
            throw lex_error(start, "Unterminated string literal");
        }
        if (c == '"') {
            advance();
            break;
        }
        value.push_back(c);
        advance();
    }
    return make (token_kind::StringLiteral, move(value), start);
}

crl::token crl::lexer::lex_ident_or_keyword() {
    source_pos start = pos_;
    size_t begin = i_;
    advance ();
    while (is_ident_count(peek())) {
        advance();
    }
    std::string text(input_.substr(begin, i_ - begin));
    
    static const std::unordered_map<std::string, token_kind> kw = {
        {"struct", token_kind::KwStruct},
        {"class", token_kind::KwClass},
        {"entity", token_kind::KwEntity},
        {"component", token_kind::KwComponent},
        {"mut", token_kind::KwMut},
        {"fn", token_kind::KwFn},
        {"let", token_kind::KwLet},
        {"ref", token_kind::KwRef},
        {"if", token_kind::KwIf},
        {"else", token_kind::KwElse},
        {"while", token_kind::KwWhile},
        {"for", token_kind::KwFor},
        {"return", token_kind::KwReturn},
        {"true", token_kind::KwTrue},
        {"false", token_kind::KwFalse},
        {"void", token_kind::KwVoid},
        {"int", token_kind::KwInt},
        {"float", token_kind::KwFloat},
        {"bool", token_kind::KwBool},
        {"string", token_kind::KwString},
    };
    auto it = kw.find(text);
    if (it == kw.end()) {
        return make (it->second, text, start);
    }
    return make (token_kind::Identifier, text, start);
}
