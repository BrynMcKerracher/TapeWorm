#include "gtest/gtest.h"
#include "../Scanner.h"
#include "../ScannerOptimiser.h"
#include "../AST/TreeBuilder.h"
#include "../AST/CompilerVisitor.h"
#include "../JIT/Compiler.h"
#include "../Util.h"
#include "../AST/TreeOptimiser.h"

std::string ExecuteBFFileWithCapturedOutput(const std::string &fileName) {
    TapeWorm::InterWorm::Scanner scanner;
    TapeWorm::ScannerOptimiser scannerOpt;
    TapeWorm::AST::TreeBuilder treeBuilder;
    TapeWorm::AST::TreeOptimiser treeOptimiser;
    TapeWorm::AST::CompilerVisitor compilerVisitor;
    TapeWorm::JIT::Compiler jitCompiler;

    const std::string fileString = TapeWorm::Util::BrainFuckFileToString(fileName);
    const auto tokens = scanner.Scan(fileString);
    const auto optTokens = scannerOpt.Optimise(tokens);
    auto ast = treeBuilder.BuildAST(optTokens);
    ast = treeOptimiser.Optimise(ast);
    const auto ops = compilerVisitor.Visit(ast);

    std::stringstream outputBuffer;
    std::streambuf* coutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(outputBuffer.rdbuf());

    jitCompiler.Compile(ops);

    std::cout.rdbuf(coutBuffer);
    return outputBuffer.str();
}

TEST(FullPipeline, Mandelbrot)
{
    const std::string programOutput = ExecuteBFFileWithCapturedOutput("bf/mandel.b");
    const std::string expectedOutput = TapeWorm::Util::FileToString("data/mandel.txt");
    EXPECT_EQ(programOutput, expectedOutput);
}

TEST(FullPipeline, HelloWorld)
{
    const std::string programOutput = ExecuteBFFileWithCapturedOutput("bf/hello.b");
    EXPECT_EQ(programOutput, "Hello World!\n");
}

TEST(FullPipeline, SerpTri)
{
    const std::string programOutput = ExecuteBFFileWithCapturedOutput("bf/serptri.b");
    const std::string expectedOutput = TapeWorm::Util::FileToString("data/serptri.txt");
    EXPECT_EQ(programOutput, expectedOutput);
}

TEST(FullPipeline, Twinkle)
{
    const std::string programOutput = ExecuteBFFileWithCapturedOutput("bf/twinkle.b");
    const std::string expectedOutput = TapeWorm::Util::FileToString("data/twinkle.txt");
    EXPECT_EQ(programOutput, expectedOutput);
}