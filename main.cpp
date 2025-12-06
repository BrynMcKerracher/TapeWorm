#include <chrono>
#include <iostream>

#include "Scanner.h"
#include "ScannerOptimiser.h"
#include "Util.h"
#include "Debug/ASTAnalyser.h"
#include "AST/CompilerVisitor.h"
#include "AST/Builder.h"
#include "AST/OptimiserVisitor.h"
#include "JIT/Compiler.h"

int main(int argc, char** argv) {
    //Check correct number of arguments
    /*
    if (argc < 2) {
        std::cout << "Usage: TapeWorm <filename>";
        return 0;
    }
    */

    TapeWorm::InterWorm::Scanner scanner;
    TapeWorm::ScannerOptimiser scannerOpt;
    TapeWorm::AST::Builder builder;
    TapeWorm::AST::CompilerVisitor compilerVisitor;
    TapeWorm::AST::OptimiserVisitor optimiser;
    TapeWorm::JIT::Compiler jitCompiler;
    TapeWorm::AST::Debug::ASTAnalyser ASTanalyser;

    //const std::string fileString = TapeWorm::Util::FileToString(argv[1]);
    const std::string fileString = TapeWorm::Util::FileToString("bf/mandelbrot.bf");
    //const std::string fileString = TapeWorm::Util::FileToString("bf/stack.bf");
    auto c1 = std::chrono::high_resolution_clock::now();

    const auto tokens = scanner.Scan(fileString);
    const auto optTokens = scannerOpt.Optimise(tokens);
    auto ast = builder.BuildAST(optTokens);
    //optimiser.Optimise(ast);
    //ASTanalyser.Analyse(ast);

    const auto ops = compilerVisitor.Visit(ast);
    auto c2 = std::chrono::high_resolution_clock::now();

    jitCompiler.Compile(ops);
    std::cout << "Optimiser: " << std::chrono::duration_cast<std::chrono::milliseconds>(c2 - c1) << "\n";

    return 0;
}
