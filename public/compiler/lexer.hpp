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
