/**
 * @file FullPipelineTest.cpp
 * @author Bryn McKerracher
 **/
#include "../Scanner.h"
#include "../Util.h"
#include "../AST/Serialiser.h"
#include "../AST/Builder.h"
#include "../AST/TreeOptimiser.h"
#include "../JIT/Compiler.h"
#include "TestEnvironmentSetup.h"

#include <iostream>
#include <catch2/catch_test_macros.hpp>

static uint64_t fnv1a64(const std::string &str) {
    uint64_t hash = 0xcbf29ce484222325;
    for (const auto ch : str) {
        constexpr uint64_t prime = 0x100000001b3;
        hash ^= ch;
        hash *= prime;
    }
    return hash;
}

TEST_CASE("Full Pipeline Tests", "[FullPipeline]") {
    TapeWorm::Test::TestEnvironmentSetup testEnvironment;
    TapeWorm::InterWorm::Scanner scanner;
    TapeWorm::AST::Builder treeBuilder;
    TapeWorm::AST::TreeOptimiser treeOptimiser;
    TapeWorm::AST::Serialiser compilerVisitor;
    TapeWorm::JIT::Compiler jitCompiler;

    SECTION("Empty") {
        const auto tokens = scanner.Scan("");
        auto ast = treeBuilder.BuildAST(tokens);
        const auto optimisedAST = treeOptimiser.Optimise(ast);
        const auto ops = compilerVisitor.Visit(optimisedAST);
        jitCompiler.Compile(ops);

        REQUIRE(fnv1a64(testEnvironment.GetProgramOutput()) == 14695981039346656037u);
    }
    SECTION("Hello World") {
        const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Programs/hw.b");
        const auto tokens = scanner.Scan(fileString);
        auto ast = treeBuilder.BuildAST(tokens);
        const auto optimisedAST = treeOptimiser.Optimise(ast);
        const auto ops = compilerVisitor.Visit(optimisedAST);
        jitCompiler.Compile(ops);

        REQUIRE(fnv1a64(testEnvironment.GetProgramOutput()) == 11208743478092974376u);
    }
    SECTION("Mandelbrot") {
        const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Programs/mandelbrot.b");
        const auto tokens = scanner.Scan(fileString);
        auto ast = treeBuilder.BuildAST(tokens);
        const auto optimisedAST = treeOptimiser.Optimise(ast);
        const auto ops = compilerVisitor.Visit(optimisedAST);
        jitCompiler.Compile(ops);

        REQUIRE(fnv1a64(testEnvironment.GetProgramOutput()) == 4686367950102506177u);
    }
    SECTION("Squares") {
        const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Programs/squares.b");
        const auto tokens = scanner.Scan(fileString);
        auto ast = treeBuilder.BuildAST(tokens);
        const auto optimisedAST = treeOptimiser.Optimise(ast);
        const auto ops = compilerVisitor.Visit(optimisedAST);
        jitCompiler.Compile(ops);

        REQUIRE(fnv1a64(testEnvironment.GetProgramOutput()) == 10922714458512192041u);
    }
    SECTION("Sierpinski") {
        const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Programs/sierpinski.b");
        const auto tokens = scanner.Scan(fileString);
        auto ast = treeBuilder.BuildAST(tokens);
        const auto optimisedAST = treeOptimiser.Optimise(ast);
        const auto ops = compilerVisitor.Visit(optimisedAST);
        jitCompiler.Compile(ops);

        REQUIRE(fnv1a64(testEnvironment.GetProgramOutput()) == 5884179497606717719u);
    }
    SECTION("Quine 1") {
        const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Programs/dquine.b");
        const auto tokens = scanner.Scan(fileString);
        auto ast = treeBuilder.BuildAST(tokens);
        const auto optimisedAST = treeOptimiser.Optimise(ast);
        const auto ops = compilerVisitor.Visit(optimisedAST);
        jitCompiler.Compile(ops);

        REQUIRE(fnv1a64(testEnvironment.GetProgramOutput()) == 12914509490304531892u);
    }
    SECTION("Quine 2") {
        const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Programs/392quine.b");
        const auto tokens = scanner.Scan(fileString);
        auto ast = treeBuilder.BuildAST(tokens);
        const auto optimisedAST = treeOptimiser.Optimise(ast);
        const auto ops = compilerVisitor.Visit(optimisedAST);
        jitCompiler.Compile(ops);

        REQUIRE(fnv1a64(testEnvironment.GetProgramOutput()) == 8767153718336888328u);
    }
    SECTION("Tic-Tac-Toe") {
        testEnvironment.CreateInputFromFile("../Programs/tictactoe.bi");
        const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Programs/tictactoe.b");
        const auto tokens = scanner.Scan(fileString);
        auto ast = treeBuilder.BuildAST(tokens);
        const auto optimisedAST = treeOptimiser.Optimise(ast);
        const auto ops = compilerVisitor.Visit(optimisedAST);
        jitCompiler.Compile(ops);

        REQUIRE(fnv1a64(testEnvironment.GetProgramOutput()) == 6775292296733711763u);
    }
    SECTION("ROT13") {
        testEnvironment.CreateInputFromFile("../Programs/rot13.bi");
        const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Programs/rot13.b");
        const auto tokens = scanner.Scan(fileString);
        auto ast = treeBuilder.BuildAST(tokens);
        const auto optimisedAST = treeOptimiser.Optimise(ast);
        const auto ops = compilerVisitor.Visit(optimisedAST);
        jitCompiler.Compile(ops);

        REQUIRE(fnv1a64(testEnvironment.GetProgramOutput()) == 15443748615009530974u);
    }
    SECTION("Bit Width") {
        const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Programs/bitwidth.b");
        const auto tokens = scanner.Scan(fileString);
        auto ast = treeBuilder.BuildAST(tokens);
        const auto optimisedAST = treeOptimiser.Optimise(ast);
        const auto ops = compilerVisitor.Visit(optimisedAST);
        jitCompiler.Compile(ops);

        REQUIRE(fnv1a64(testEnvironment.GetProgramOutput()) == 0x4fe3f16f029c6f00);
    }
    SECTION("X-Mas Tree") {
        const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Programs/xmastree.b");
        const auto tokens = scanner.Scan(fileString);
        auto ast = treeBuilder.BuildAST(tokens);
        const auto optimisedAST = treeOptimiser.Optimise(ast);
        const auto ops = compilerVisitor.Visit(optimisedAST);
        jitCompiler.Compile(ops);

        REQUIRE(fnv1a64(testEnvironment.GetProgramOutput()) == 0x7e7be07b4abe035);
    }
}
