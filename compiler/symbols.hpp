#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

#include "ast.hpp"
#include "ids.hpp"

namespace cpl {

    enum class SymbolKind {
        Module,
        Type,     // class/interface/component/entity
        Function,
        Field,
        Var,
        Param,
      };

    enum class TypeDeclKind {
        Class,
        Interface,
        Component,
        Entity,
      };

    struct Symbol {
        SymbolKind kind;
        std::string name;
        ScopeId scope = kInvalidId;     // 所属スコープ
        SourcePos pos{};               // どこで宣言されたか（ast.hppからSourcePosを共有したければ移動）
    };

    struct TypeSymbolData {
        TypeDeclKind declKind;
        // interface 実装（適用済みTypeIdの配列）
        std::vector<TypeId> implements;
        // ジェネリック情報や where 制約もここにぶら下げる
    };

    struct FunctionSymbolData {
        TypeId functionType = kInvalidId;
        // ジェネリック + where も保持
    };

    struct Scope {
        ScopeId parent = kInvalidId;
        std::unordered_map<std::string, SymbolId> table; // v0は1名1シンボル（オーバーロード無し想定）
    };

    struct SymbolTable {
        std::vector<Symbol> symbols;
        std::vector<Scope> scopes;

        ScopeId newScope(ScopeId parent) {
            scopes.push_back(Scope{.parent = parent});
            return static_cast<ScopeId>(scopes.size() - 1);
        }

        SymbolId addSymbol(ScopeId scope, Symbol s) {
            symbols.push_back(std::move(s));
            SymbolId id = static_cast<SymbolId>(symbols.size() - 1);
            scopes[scope].table[symbols[id].name] = id;
            return id;
        }

        std::optional<SymbolId> lookup(ScopeId scope, const std::string& name) const {
            for (ScopeId cur = scope; cur != kInvalidId; cur = scopes[cur].parent) {
                auto it = scopes[cur].table.find(name);
                if (it != scopes[cur].table.end()) return it->second;
            }
            return std::nullopt;
        }
    };

} // namespace cpl