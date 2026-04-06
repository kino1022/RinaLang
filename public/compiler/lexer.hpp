#pragma once
#include <string>
#include <vector>

namespace crl {
    
    enum class token_kind {
        Eof,
        
        Identifier,
        IntLiteral,
        FloatLiteral,
        BoolLiteral,
        StringLiteral,
        
        KwStruct,
        KwClass,
        KwEntity,
        KwComponent,
        KwFn,
        KwLet,
        KwMut,
        KwRef,
        KwReturn,
        KwTrue,
        KwFalse,
        
        KwInt,
        KwBool,
        KwFloat,
        KwString,
        KwVoid,
        
        KwIf,
        KwElse,
        KwWhile,
        KwFor,
        
        
        LBrace,
        RBrace,
        LParen,
        RParen,
        Comma,
        Colon,
        SemiColon,
        Dot,
        Assign,
        Arrow,
        Plus,
        Minus,
        EqualEqual, // ==
        NotEqual, // !=
    };
    
    static const char* token_kind_name(crl::token_kind k) {
        using crl::token_kind;
        switch (k) {
            case token_kind::Eof: return "Eof";
            case token_kind::Identifier: return "Identifier";
            case token_kind::IntLiteral: return "IntLiteral";
            case token_kind::FloatLiteral: return "FloatLiteral";
            case token_kind::BoolLiteral: return "BoolLiteral";
            case token_kind::StringLiteral: return "StringLiteral";

            case token_kind::KwStruct: return "KwStruct";
            case token_kind::KwClass: return "KwClass";
            case token_kind::KwEntity: return "KwEntity";
            case token_kind::KwComponent: return "KwComponent";
            case token_kind::KwFn: return "KwFn";
            case token_kind::KwLet: return "KwLet";
            case token_kind::KwMut: return "KwMut";
            case token_kind::KwRef: return "KwRef";
            case token_kind::KwReturn: return "KwReturn";
            case token_kind::KwTrue: return "KwTrue";
            case token_kind::KwFalse: return "KwFalse";

            case token_kind::KwInt: return "KwInt";
            case token_kind::KwBool: return "KwBool";
            case token_kind::KwFloat: return "KwFloat";
            case token_kind::KwString: return "KwString";
            case token_kind::KwVoid: return "KwVoid";

            case token_kind::KwIf: return "KwIf";
            case token_kind::KwElse: return "KwElse";
            case token_kind::KwWhile: return "KwWhile";
            case token_kind::KwFor: return "KwFor";

            case token_kind::LBrace: return "LBrace";
            case token_kind::RBrace: return "RBrace";
            case token_kind::LParen: return "LParen";
            case token_kind::RParen: return "RParen";
            case token_kind::Comma: return "Comma";
            case token_kind::Colon: return "Colon";
            case token_kind::SemiColon: return "SemiColon";
            case token_kind::Dot: return "Dot";
            case token_kind::Assign: return "Assign";
            case token_kind::Arrow: return "Arrow";
            case token_kind::Plus: return "Plus";
            case token_kind::Minus: return "Minus";
            case token_kind::EqualEqual: return "EqualEqual";
            case token_kind::NotEqual: return "NotEqual";
            default: return "Unknown";
        }
    }
    
    struct source_pos {
        std::string source_path;
        int line;
        int column;
    };
    
    struct token {
        token_kind kind;
        // raw text (for indent,string,int; also useful for error message)
        std::string lexeme;
        // start position
        source_pos pos;
    };
    
    class lex_error : public std::runtime_error {
    public :
        source_pos pos;
        lex_error(source_pos p, const std::string& message) : std::runtime_error(message), pos(p) {}
    };
    
    class lexer {
    public:
        
        explicit lexer (std::string_view input, const std::string& filepath) : input_(input),filepath_(filepath) {}
        
        std::vector<token> lex_all ();
        
        token next ();
        
    private:
        
        std::string_view input_;
        
        std::string filepath_;
        
        size_t i_ = 0;
        
        source_pos pos_{filepath_, 1,1};
        
        char peek (size_t lookahead = 0) const;
        
        void advance ();
        
        token make (token_kind kind, std::string_view lexeme, source_pos pos);
        
        static bool is_ident_start (char c);
        
        static bool is_ident_count (char c);
        
        void skip_comment_and_space();
        
        token lex_int();
        
        token lex_float();
        
        token lex_string();

        token lex_ident_or_keyword();
    };
}
