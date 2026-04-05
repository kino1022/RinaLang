# pragma once
#include <cstdint>

namespace cpl {
    
    using FileId = uint32_t;
    using ModuleId = uint32_t;
    
    using DeclId = uint32_t;
    using StmtId = uint32_t;
    using ExprId = uint32_t;
    
    using ScopeId = uint32_t;
    using SymbolId = uint32_t;
    
    using TypeId = uint32_t;
    
    constexpr uint32_t kInvalidId = 0xFFFFFFFFu;
    
}