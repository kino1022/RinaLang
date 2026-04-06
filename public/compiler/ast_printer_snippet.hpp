#pragma once
#include "parser.hpp"

#include <sstream>
#include <string>
#include <type_traits>
#include <variant>

namespace crl {

inline std::string indent_str(int n) { return std::string(n * 2, ' '); }

inline std::string print_expr(const expr& e, int indent = 0); // forward

inline std::string print_arg(const arg& a, int indent) {
    std::ostringstream oss;

    if (a.kind == arg::kind_t::Ref) {
        oss << indent_str(indent) << "Ref(" << a.ref_place->root;
        for (auto& f : a.ref_place->fields) oss << "." << f;
        oss << ")";
        return oss.str();
    }

    oss << indent_str(indent) << "Value(\n";
    oss << print_expr(*a.value, indent + 1) << "\n";
    oss << indent_str(indent) << ")";
    return oss.str();
}

inline std::string print_expr(const expr& e, int indent) {
    std::ostringstream oss;

    std::visit([&](auto&& node) {
        using T = std::decay_t<decltype(node)>;

        if constexpr (std::is_same_v<T, expr_int>) {
            oss << indent_str(indent) << "Int(" << node.tok.lexeme << ")";
        } else if constexpr (std::is_same_v<T, expr_var>) {
            oss << indent_str(indent) << "Var(" << node.tok.lexeme << ")";
        } else if constexpr (std::is_same_v<T, expr_field>) {
            oss << indent_str(indent) << "Field(\n";
            oss << print_expr(*node.base, indent + 1) << ",\n";
            oss << indent_str(indent + 1) << node.field.lexeme << "\n";
            oss << indent_str(indent) << ")";
        } else if constexpr (std::is_same_v<T, expr_call>) {
            oss << indent_str(indent) << "Call(\n";
            oss << print_expr(*node.callee, indent + 1);

            if (!node.args.empty()) {
                oss << ",\n" << indent_str(indent + 1) << "Args[\n";
                for (size_t i = 0; i < node.args.size(); i++) {
                    oss << print_arg(node.args[i], indent + 2);
                    if (i + 1 != node.args.size()) oss << ",\n";
                }
                oss << "\n" << indent_str(indent + 1) << "]";
            }

            oss << "\n" << indent_str(indent) << ")";
        }
    }, e.node);

    return oss.str();
}

} // namespace crl