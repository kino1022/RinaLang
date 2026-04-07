#include <iostream>

#include "public/compiler/file_read.hpp"
#include "public/compiler/lexer.hpp"
#include "public/compiler/parser.hpp"
#include "public/compiler/ast_printer_snippet.hpp"

// TIP コードを<b>Run</b>するには、<shortcut actionId="Run"/> を押すか、ガターにある <icon src="AllIcons.Actions.Execute"/> アイコンをクリックします。
int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "usage : rinalang <file>\n";
        return 1;
    }
    const std::string mode = argv[1];
    const std::string path = argv[2];
    try {
        std::string src = read_entire_file(path);
        crl::lexer lx(src, path);
        auto toks = lx.lex_all();
        if (mode == "--lex") {
            for (const auto& t : toks) {
                std::cout << t.pos.source_path << ":" << t.pos.line << ":" << t.pos.column
                            << " " << crl::token_kind_name(t.kind)
                            << "[" << t.lexeme << "]";
            }
            return 0;
        }
        if (mode == "--parse-place") {
            crl::parser ps(toks);
            crl::place p = ps.parse_place();
            std::cout << "place.root = " << p.root << std::endl;
            for (size_t i = 0; i < p.fields.size(); i++) {
                std::cout << "place.field[" << i << "] = " << p.fields[i] << std::endl;
            }
            return 0;
        }
        if (mode == "--parse-call") {
            
        }
        if (mode == "--parse-expr") {
            crl::parser ps(toks);
            crl::expr_ptr e = ps.parse_call_expr(); // 既存名のまま使うならこれ
            std::cout << print_expr(*e) << "\n";
            return 0;
        }
        if (mode == "--parse-stmt") {
            crl::parser ps(toks);
            crl::stmt_ptr s = ps.parse_stmt_eof();
            std::cout << print_stmt(*s) << "\n";
            return 0;
        }

        if (mode == "--parse-block") {
            crl::parser ps(toks);
            crl::block b = ps.parse_block_eof();
            std::cout << print_block(b) << "\n";
            return 0;
        }
        
        if (mode == "--parse-fn") {
            crl::parser ps(toks);
            crl::fn_item f = ps.parse_fn_eof();
            std::cout << print_fn(f) << "\n";
            return 0;
        }
    }
    catch (const crl::lex_error& e) {
        std::cerr 
            << e.pos.source_path << ":" << e.pos.line << ":" << e.pos.column
            << ": " << e.what() << std::endl;
        return 2;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 3;
    }
}