#include <chrono>
#include <iostream>

#include "Scanner.h"
#include "Util.h"
#include "AST/CompilerVisitor.h"
#include "AST/Builder.h"
#include "JIT/Compiler.h"

int main(int argc, char** argv) {
    //Check correct number of arguments
    if (argc < 2) {
        std::cerr << "Error: No file given to interpret.\n";
        std::cerr << "Example Usage: './TapeWorm <filename>\n";
        return 0;
    }

    TapeWorm::InterWorm::Scanner scanner;
    TapeWorm::AST::Builder treeBuilder;
    TapeWorm::AST::CompilerVisitor compilerVisitor;
    TapeWorm::JIT::Compiler jitCompiler;

    const std::string fileString = TapeWorm::Util::BrainFuckFileToString(argv[1]);
    const auto tokens = scanner.Scan(fileString);
    auto ast = treeBuilder.BuildAST(tokens);
    const auto ops = compilerVisitor.Visit(ast);

    jitCompiler.Compile(ops);

    return 0;
}
