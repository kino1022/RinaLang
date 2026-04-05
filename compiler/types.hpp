#pragma once
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include "ids.hpp"

namespace cpl {
    enum class PrimitiveKind {
        Void,
        Bool,
        I32,
        I64,
        F32,
        F64,
        String,
    };
    
    struct Type;
    
    struct FunctionType {
        std::vector<TypeId> params;
        TypeId ret = kInvalidId;
    };
    
    struct NominalType {
        SymbolId symbol = kInvalidId;
    };

    struct GenericParamType {
        uint32_t index = 0;
    };
    
    struct AppliedType {
        TypeId base = kInvalidId;
        std::vector<TypeId> args;
    };
    
    enum class TypeKind {
        Primitive,
        Function,
        Nominal,
        GenericParam,
        Applied,
    };
    
    struct Type {
        std::vector<Type> types;
        
        TypeId add (Type t) {
            types.push_back(std::move(t));
            return static_cast<TypeId>(types.size() - 1);
        }
        
        const Type& operator[](TypeId id) const { return types[id]; }
        Type& operator[](TypeId id) {   return types[id];   }
    };
}