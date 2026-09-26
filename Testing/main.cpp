/**
 * @file Testing/main.cpp
 * @author Bryn McKerracher
 **/
#include "../Scanner.h"
#include "../Util.h"
#include "../AST/CompilerVisitor.h"
#include "../AST/Builder.h"
#include "../JIT/Compiler.h"

#include <iostream>
#include <sstream>

static uint64_t fnv1a64(const std::string &str) {
    uint64_t hash = 0xcbf29ce484222325;
    for (const auto ch : str) {
        constexpr uint64_t prime = 0x100000001b3;
        hash ^= ch;
        hash *= prime;
    }
    return hash;
}

int main(const int argc, char** argv) {
    std::streambuf* coutbuf = std::cout.rdbuf();

    std::stringstream stream;
    std::cout.rdbuf(stream.rdbuf());

    TapeWorm::InterWorm::Scanner scanner;
    TapeWorm::AST::Builder treeBuilder;
    TapeWorm::AST::CompilerVisitor compilerVisitor;
    TapeWorm::JIT::Compiler jitCompiler;

    const std::string fileString = TapeWorm::Util::BrainFuckFileToString(argv[1]);
    const auto tokens = scanner.Scan(fileString);
    const auto ast = treeBuilder.BuildAST(tokens);
    const auto ops = compilerVisitor.Visit(ast);

    jitCompiler.Compile(ops);

    std::cout.rdbuf(coutbuf);
    std::cout << fnv1a64(stream.str());

    return 0;
}