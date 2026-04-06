#include <iostream>

#include "public/compiler/file_read.hpp"
#include "public/compiler/lexer.hpp"

// TIP コードを<b>Run</b>するには、<shortcut actionId="Run"/> を押すか、ガターにある <icon src="AllIcons.Actions.Execute"/> アイコンをクリックします。
int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage : rinalang <file>\n";
        return 1;
    }
    const std::string path = argv[1];
    try {
        std::string src = read_entire_file(path);
        crl::lexer lx(src, path);
        auto toks = lx.lex_all();
        for (const auto& t : toks) {
            std::cout << t.pos.source_path << ":" << t.pos.line << ":" << t.pos.column
                        << " " << crl::token_kind_name(t.kind)
                        << "[" << t.lexeme << "]";
        }
        return 0;
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