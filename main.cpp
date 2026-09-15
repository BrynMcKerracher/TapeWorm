#include <chrono>
#include <iostream>

#include "Scanner.h"
#include "ScannerOptimiser.h"
#include "Util.h"
#include "Debug/ASTAnalyser.h"
#include "AST/CompilerVisitor.h"
#include "AST/TreeBuilder.h"
#include "AST/TreeOptimiser.h"
#include "JIT/Compiler.h"

int main(int argc, char** argv) {
    //Check correct number of arguments
    /*if (argc < 2) {
        std::cout << "Usage: TapeWorm <filename>";
        return 0;
    } */
    std::string fileName = "BF/hello.b";

    TapeWorm::InterWorm::Scanner scanner;
    TapeWorm::ScannerOptimiser scannerOpt;
    TapeWorm::AST::TreeBuilder treeBuilder;
    TapeWorm::AST::TreeOptimiser treeOptimiser;
    TapeWorm::AST::CompilerVisitor compilerVisitor;
    TapeWorm::JIT::Compiler jitCompiler;
    TapeWorm::AST::Debug::ASTAnalyser ASTanalyser;

    const std::string fileString = TapeWorm::Util::BrainFuckFileToString(fileName);
    //const std::string fileString = TapeWorm::Util::BrainFuckFileToString(argv[1]);
    auto c1 = std::chrono::high_resolution_clock::now();

    const auto tokens = scanner.Scan(fileString);
    //const auto optTokens = scannerOpt.Optimise(tokens);
    auto ast = treeBuilder.BuildAST(tokens);
    //ast = treeOptimiser.Optimise(ast);
    //ASTanalyser.Analyse(ast);

    const auto ops = compilerVisitor.Visit(ast);
    auto c2 = std::chrono::high_resolution_clock::now();

    jitCompiler.Compile(ops);
    std::cout << "Optimiser: " << std::chrono::duration_cast<std::chrono::milliseconds>(c2 - c1) << "\n";

    return 0;
}