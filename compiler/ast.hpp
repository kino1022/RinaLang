#pragma once
#include <string>
#include <vector>
#include <variant>
#include <optional>

#include "ids.hpp"
#include "compiler/ids.hpp"

namespace cpl {

    struct SourcePos {
        FileId file = kInvalidId;
        uint32_t line = 1;
        uint32_t col = 1;
    };
    
    struct QualifiedName {
        std::vector<std::string> parts;
    };
    
    struct TypeRefAST;
    using TypeRefId = uint32_t;
    
    struct TypeRefAST {
        SourcePos pos;
        QualifiedName name;
        std::vector<TypeRefId> args;
    };
    
    struct WhereItemAST {
        SourcePos pos;
        std::string paramName;
        std::vector<TypeRefId> bounds;
    };
    
    struct WhereClauseAST {
        SourcePos pos;
        std::vector<WhereItemAST> items;
    };
    
    struct GenericParamsAST {
        SourcePos pos;
        std::vector<std::string> params;
    };

    struct ParamAST {
        SourcePos pos;
        std::string name;
        TypeRefId type;
    };
    
    struct FunctionSigAST {
        SourcePos pos;
        std::string name;
        std::optional<GenericParamsAST> generics;
        std::vector<ParamAST> params;
        TypeRefId returnType;
        std::optional<WhereClauseAST> where;
    };
    
    struct InterfaceDeclAST {
        SourcePos pos;
        std::string name;
        std::optional<GenericParamsAST> generics;
        std::vector<FunctionSigAST> methods;
    };
    
    struct FieldDeclAST {
        SourcePos pos;
        bool isVar = true; // var/let
        std::string name;
        TypeRefId type;
        std::optional<ExprId> init;
    };

    struct FunctionDeclAST {
        SourcePos pos;
        FunctionSigAST sig;
        // body
        std::vector<StmtId> body; // Block内StmtIdの列（Block自体をStmtにしてもOK）
    };

    struct ClassDeclAST {
        SourcePos pos;
        std::string name;
        std::optional<GenericParamsAST> generics;
        std::optional<WhereClauseAST> where;
        std::vector<std::variant<FieldDeclAST, FunctionDeclAST>> members;
    };

    struct ComponentDeclAST {
        SourcePos pos;
        std::string name;
        std::optional<GenericParamsAST> generics;
        std::optional<WhereClauseAST> where;
        std::vector<std::variant<FieldDeclAST, FunctionDeclAST>> members;
    };

    struct EntityDeclAST {
        SourcePos pos;
        std::string name;
        std::optional<GenericParamsAST> generics;
        std::vector<TypeRefId> implements; // interface refs
        std::optional<WhereClauseAST> where;
        std::vector<std::variant<FieldDeclAST, FunctionDeclAST>> members;
    };

    using TopDeclAST = std::variant<
      InterfaceDeclAST, ClassDeclAST, ComponentDeclAST, EntityDeclAST, FunctionDeclAST
    >;

    struct FileAST {
        SourcePos pos;
        QualifiedName moduleName;
        std::vector<QualifiedName> imports;
        std::vector<TopDeclAST> decls;

        // TypeRefはファイル単位で集約してID参照にすると楽
        std::vector<TypeRefAST> typeRefs;
    };
}