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
#include <exception>

#include <gtest/gtest.h>

namespace {
    class FullPipelineTest : public ::testing::Test {
    protected:
        void SetUp() override {
            coutbuf = std::cout.rdbuf();
            std::cout.rdbuf(stream.rdbuf());
        }

        void TearDown() override {
            std::cout.rdbuf(coutbuf);
        }

        std::streambuf* coutbuf = nullptr;
        std::stringstream stream;

        TapeWorm::InterWorm::Scanner scanner;
        TapeWorm::AST::Builder treeBuilder;
        TapeWorm::AST::CompilerVisitor compilerVisitor;
        TapeWorm::JIT::Compiler jitCompiler;
    };
}

static uint64_t fnv1a64(const std::string &str) {
    uint64_t hash = 0xcbf29ce484222325;
    for (const auto ch : str) {
        constexpr uint64_t prime = 0x100000001b3;
        hash ^= ch;
        hash *= prime;
    }
    return hash;
}

TEST_F(FullPipelineTest, HelloWorld) {
    const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Tests/hw.b");
    const auto tokens = scanner.Scan(fileString);
    const auto ast = treeBuilder.BuildAST(tokens);
    const auto ops = compilerVisitor.Visit(ast);

    try {
        jitCompiler.Compile(ops);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    EXPECT_EQ(fnv1a64(stream.str()), 11208743478092974376u);
}

TEST_F(FullPipelineTest, Mandelbrot) {
    const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Tests/mandelbrot.b");
    const auto tokens = scanner.Scan(fileString);
    const auto ast = treeBuilder.BuildAST(tokens);
    const auto ops = compilerVisitor.Visit(ast);

    jitCompiler.Compile(ops);

    EXPECT_EQ(fnv1a64(stream.str()), 4686367950102506177u);
}

TEST_F(FullPipelineTest, BitWidth) {
    const std::string fileString = TapeWorm::Util::BrainFuckFileToString("../Tests/bitwidth.b");
    const auto tokens = scanner.Scan(fileString);
    const auto ast = treeBuilder.BuildAST(tokens);
    const auto ops = compilerVisitor.Visit(ast);

    jitCompiler.Compile(ops);

    EXPECT_EQ(fnv1a64(stream.str()), 5756710207797096192u);
}