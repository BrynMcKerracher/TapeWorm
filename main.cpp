/**
 * @file main.cpp
 * @author Bryn McKerracher
 **/
#include "Scanner.h"
#include "Util.h"
#include "AST/CompilerVisitor.h"
#include "AST/Builder.h"
#include "JIT/Compiler.h"
#include "AST/TreeOptimiser.h"
#include "CLI/OptionsManagement.h"
#include "CLI/Environment.h"

#include <iostream>

#include "IROptimiser.h"

static void PrintHelp(const TapeWorm::Environment &env) {
    std::cout << "-------------------------------------------\n";
    std::cout << " TapeWorm " << env.Version << "\n";
    std::cout << "-------------------------------------------\n";
    std::cout << " To run a brainfuck file, use:\n";
    std::cout << "\tLinux:\t\t./TapeWorm example.b\n";
    std::cout << "\tWindows:\tTapeWorm.exe example.b\n";
    std::cout << "-------------------------------------------\n";
    std::cout << " Command Line Arguments (These are optional):\n";
    std::cout << "\t--no-ast-opt\tSkips AST optimisation phase.\n";
    std::cout << "-------------------------------------------\n";
}

int main(const int argc, char** argv) {
    //Check correct number of arguments
    if (argc < 2) {
        std::cerr << "Error: No source file given.\n";
        std::cerr << "Please make sure to reference a brainfuck source file.\n";
        std::cerr << "Example Usage: './TapeWorm filename.b'\n";
        std::cerr << "For help, use './TapeWorm --help'\n";
        return 0;
    }

    TapeWorm::CLI::OptionsManagement options;
    const TapeWorm::Environment environment = options.ParseCommands(argc, argv);
    if (environment.InvalidEnvironment) {
        PrintHelp(environment);
        return 0;
    }

    TapeWorm::InterWorm::Scanner scanner;
    TapeWorm::AST::Builder treeBuilder;
    TapeWorm::AST::CompilerVisitor compilerVisitor;
    TapeWorm::JIT::Compiler jitCompiler;

    const std::string fileString = TapeWorm::Util::BrainFuckFileToString(environment.SourceFileLocation);
    const auto tokens = scanner.Scan(fileString);
    auto ast = treeBuilder.BuildAST(tokens);
    if (environment.PerformASTOptimisationPass) {
        TapeWorm::AST::TreeOptimiser treeOptimiser;
        ast = treeOptimiser.Optimise(ast);
    }
    auto ops = compilerVisitor.Visit(ast);
    if (environment.PerformIROptimisationPass) {
        TapeWorm::InterWorm::IROptimiser irOptimiser;
        ops = irOptimiser.Optimise(ops);
    }
    jitCompiler.Compile(ops);

    return 0;
}
