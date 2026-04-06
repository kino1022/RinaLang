#pragma once

namespace crl {
    
    enum class token_kind {
        Eof,
        
        Identifier,
        IntLiteral,
        FloatLiteral,
        BoolLiteral,
        StringLiteral,
    };
    
    class token {
        string source_path;
    };
}