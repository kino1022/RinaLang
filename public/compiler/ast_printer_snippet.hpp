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

    inline std::string print_type(const crl::type& t) {
        std::ostringstream oss;
        std::visit([&](auto&& node) {
            using T = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<T, crl::type_named>) {
                oss << node.name.lexeme;
            }
            else if constexpr (std::is_same_v<T, crl::type_ref>) {
                oss << "ref " << print_type(*node.base);
            }
        }, t.node);
        return oss.str();
    }


    inline std::string print_expr(const expr& e, int indent) {
        std::ostringstream oss;

        std::visit([&](auto&& node) {
            using T = std::decay_t<decltype(node)>;

            if constexpr (std::is_same_v<T, expr_int>) {
                oss << indent_str(indent) << "Int(" << node.tok.lexeme << ")";
            }
            else if constexpr (std::is_same_v<T, expr_var>) {
                oss << indent_str(indent) << "Var(" << node.tok.lexeme << ")";
            }
            else if constexpr (std::is_same_v<T, expr_field>) {
                oss << indent_str(indent) << "Field(\n";
                oss << print_expr(*node.base, indent + 1) << ",\n";
                oss << indent_str(indent + 1) << node.field.lexeme << "\n";
                oss << indent_str(indent) << ")";
            }
            else if constexpr (std::is_same_v<T, expr_call>) {
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
            else if constexpr (std::is_same_v<T, expr_string>) {
                oss << indent_str(indent) << "String\"" << node.tok.lexeme << "\")";
            }
            else if constexpr (std::is_same_v<T, expr_bool>) {
                oss << indent_str(indent) << "Bool(" << (node.value ? "true" : "false") << ")";
            }
            else if constexpr (std::is_same_v<T, crl::expr_binary>) {
                oss << indent_str(indent) << "Binary(\n";
                oss << indent_str(indent + 1) << "op=" << node.op.lexeme << ",\n";
                oss << print_expr(*node.lhs, indent + 1) << ",\n";
                oss << print_expr(*node.rhs, indent + 1) << "\n";
                oss << indent_str(indent) << ")";
            }
            else if constexpr (std::is_same_v<T, crl::expr_unary>) {
                oss << indent_str(indent) << "Unary(\n";
                oss << indent_str(indent + 1) << "op=" << node.op.lexeme << ",\n";
                oss << print_expr(*node.rhs, indent + 1) << "\n";
                oss << indent_str(indent) << ")";
            }
        }, e.node);

        return oss.str();
    }

    inline std::string print_stmt(const crl::stmt& s, int indent = 0);
    inline std::string print_block(const crl::block& b, int indent = 0);

    inline std::string print_stmt(const crl::stmt& s, int indent) {
        std::ostringstream oss;
        std::visit([&](auto&& node) {
            using T = std::decay_t<decltype(node)>;

            // print_stmt の stmt_let 分岐をこれに置換
            if constexpr (std::is_same_v<T, crl::stmt_let>) {
                oss << indent_str(indent) << "Let(mut=" << (node.is_mut ? "true" : "false")
                    << ", name=" << node.name.lexeme;

                if (node.ty.has_value()) {
                    oss << ", ty=" << print_type(*(*node.ty)); // optional<type_ptr>
                }

                oss << ",\n";
                oss << print_expr(*node.init, indent + 1) << "\n";
                oss << indent_str(indent) << ")";
            }
            else if constexpr (std::is_same_v<T, crl::stmt_assign>) {
                oss << indent_str(indent) << "Assign(lhs=" << node.lhs.root;
                for (auto& f : node.lhs.fields) oss << "." << f;
                oss << ",\n";
                oss << print_expr(*node.rhs, indent + 1) << "\n";
                oss << indent_str(indent) << ")";
            }
            else if constexpr (std::is_same_v<T, crl::stmt_return>) {
                oss << indent_str(indent) << "Return(";
                if (!node.value.has_value()) {
                    oss << ")";
                }
                else {
                    oss << "\n" << print_expr(*(*node.value), indent + 1) << "\n"
                        << indent_str(indent) << ")";
                }
            }
            else if constexpr (std::is_same_v<T, crl::stmt_expr>) {
                oss << indent_str(indent) << "ExprStmt(\n";
                oss << print_expr(*node.value, indent + 1) << "\n";
                oss << indent_str(indent) << ")";
            }
        }, s.node);
        return oss.str();
    }

    inline std::string print_block(const crl::block& b, int indent) {
        std::ostringstream oss;
        oss << indent_str(indent) << "Block[\n";
        for (size_t i = 0; i < b.stmts.size(); i++) {
            oss << print_stmt(*b.stmts[i], indent + 1);
            if (i + 1 != b.stmts.size()) oss << ",\n";
        }
        oss << "\n" << indent_str(indent) << "]";
        return oss.str();
    }

    inline std::string print_type_name(const crl::type_named& t) {
        return t.name.lexeme;
    }

    inline std::string print_fn(const crl::fn_item& f, int indent = 0) {
        std::ostringstream oss;
        oss << indent_str(indent) << "Fn(name=" << f.name.lexeme << ", params=[";

        for (size_t i = 0; i < f.params.size(); i++) {
            const auto& p = f.params[i];
            oss << p.name.lexeme;
            if (p.ty.has_value()) oss << ":" << print_type(*(*p.ty));
            if (i + 1 != f.params.size()) oss << ", ";
        }

        oss << "]";
        if (f.ret_type.has_value()) oss << ", ret=" << print_type(*(*f.ret_type));
        oss << ",\n";
        oss << print_block(f.body, indent + 1) << "\n";
        oss << indent_str(indent) << ")";
        return oss.str();
    }
} // namespace crl
