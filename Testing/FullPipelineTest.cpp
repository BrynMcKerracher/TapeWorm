/**
 * @file FullPipelineTest.cpp
 * @author Bryn McKerracher
 **/
#include "../Scanner.h"
#include "../Util.h"
#include "../AST/CompilerVisitor.h"
#include "../AST/Builder.h"
#include "../JIT/Compiler.h"

#include <iostream>
#include <sstream>
#include <catch2/catch_test_macros.hpp>

uint64_t fnv1a64(const std::string &str) {
    uint64_t hash = 0xcbf29ce484222325;
    for (const auto ch : str) {
        constexpr uint64_t prime = 0x100000001b3;
        hash ^= ch;
        hash *= prime;
    }
    return hash;
}

TEST_CASE("Full Pipline Tests", "[FullPipeline]") {
    TapeWorm::InterWorm::Scanner scanner;
    TapeWorm::AST::Builder treeBuilder;
    TapeWorm::AST::CompilerVisitor compilerVisitor;
    TapeWorm::JIT::Compiler jitCompiler;
    std::stringstream stream;
    std::streambuf* coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(stream.rdbuf());

    SECTION("Hello World") {
        const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Tests/hw.b");
        const auto tokens = scanner.Scan(fileString);
        const auto ast = treeBuilder.BuildAST(tokens);
        const auto ops = compilerVisitor.Visit(ast);

        jitCompiler.Compile(ops);

        REQUIRE(fnv1a64(stream.str()) == 11208743478092974376u);
    }
    SECTION("Mandelbrot") {
        const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Tests/mandelbrot.b");
        const auto tokens = scanner.Scan(fileString);
        const auto ast = treeBuilder.BuildAST(tokens);
        const auto ops = compilerVisitor.Visit(ast);

        jitCompiler.Compile(ops);

        REQUIRE(fnv1a64(stream.str()) == 4686367950102506177u);
    }
    SECTION("Bitwidth") {
        const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Tests/bitwidth.b");
        const auto tokens = scanner.Scan(fileString);
        const auto ast = treeBuilder.BuildAST(tokens);
        const auto ops = compilerVisitor.Visit(ast);

        jitCompiler.Compile(ops);

        REQUIRE(fnv1a64(stream.str()) == 5756710207797096192u);
    }
    SECTION("Squares") {
        const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Tests/squares.b");
        const auto tokens = scanner.Scan(fileString);
        const auto ast = treeBuilder.BuildAST(tokens);
        const auto ops = compilerVisitor.Visit(ast);

        jitCompiler.Compile(ops);

        REQUIRE(fnv1a64(stream.str()) == 10922714458512192041u);
    }
    SECTION("Sierpinski") {
        const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Tests/sierpinski.b");
        const auto tokens = scanner.Scan(fileString);
        const auto ast = treeBuilder.BuildAST(tokens);
        const auto ops = compilerVisitor.Visit(ast);

        jitCompiler.Compile(ops);

        REQUIRE(fnv1a64(stream.str()) == 5884179497606717719u);
    }
    SECTION("Quine 1") {
        const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Tests/dquine.b");
        const auto tokens = scanner.Scan(fileString);
        const auto ast = treeBuilder.BuildAST(tokens);
        const auto ops = compilerVisitor.Visit(ast);

        jitCompiler.Compile(ops);

        REQUIRE(fnv1a64(stream.str()) == 12914509490304531892u);
    }
    SECTION("Quine 2") {
        const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Tests/392quine.b");
        const auto tokens = scanner.Scan(fileString);
        const auto ast = treeBuilder.BuildAST(tokens);
        const auto ops = compilerVisitor.Visit(ast);

        jitCompiler.Compile(ops);

        REQUIRE(fnv1a64(stream.str()) == 8767153718336888328u);
    }
    std::cout.rdbuf(coutbuf);
}