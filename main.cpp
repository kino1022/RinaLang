#include <iostream>

#include "public/compiler/file_read.hpp"
#include "public/compiler/lexer.hpp"
#include "public/compiler/parser.hpp"

// TIP コードを<b>Run</b>するには、<shortcut actionId="Run"/> を押すか、ガターにある <icon src="AllIcons.Actions.Execute"/> アイコンをクリックします。
int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage : rinalang <file>\n";
        return 1;
    }
    const std::string path = argv[2];
    const std::string mode = argv[1];
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
            crl::parser ps(toks);
            crl::call_expr c = ps.parse_call();
            std::cout << "call.callee = " << c.callee << std::endl;
            for (size_t i = 0; i < c.args.size(); i++) {
                const auto& a = c.args[i];
                if (a.kind == crl::arg::kind_t::Ref) {
                    std::cout << "arg[" << i << "] = ref "
                            << a.ref_place->root;
                    for (auto& f : a.ref_place->fields) std::cout << "." << f;
                    std::cout << "\n";
                } else {
                    std::cout << "arg[" << i << "] = value "
                            << a.value_token->lexeme << "\n";
                }
            }
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