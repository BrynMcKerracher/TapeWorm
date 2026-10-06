/**
 * @file BenchmarksFullPipeline.cpp
 * @author Bryn McKerracher
 **/
#include "../TapeWorm.h"
#include "../Testing/TestEnvironmentSetup.h"

#include <filesystem>
#include <nanobench.h>

static void RunBenchmark(const std::string& testFile, const std::string& testName) {
    TapeWorm::InterWorm::Scanner scanner;
    TapeWorm::AST::Builder treeBuilder;
    TapeWorm::AST::CompilerVisitor compilerVisitor;

    const std::string fileString = TapeWorm::Util::BrainFuckFileToString(testFile);
    const auto tokens = scanner.Scan(fileString);
    auto ast = treeBuilder.BuildAST(tokens);
    TapeWorm::AST::TreeOptimiser treeOptimiser;
    ast = treeOptimiser.Optimise(ast);
    auto ops = compilerVisitor.Visit(ast);
    TapeWorm::InterWorm::IROptimiser irOptimiser;
    ops = irOptimiser.Optimise(ops);

    ankerl::nanobench::Bench().epochs(100).minEpochIterations(20).run(testName, [&] {
        TapeWorm::Test::TestEnvironmentSetup setup;
        TapeWorm::JIT::Compiler jitCompiler;
        jitCompiler.Compile(ops);
    });
}

int main() {
    for (const auto & entry : std::filesystem::directory_iterator("../Programs")) {
        if (entry.path().extension() == ".b") {
            RunBenchmark(entry.path(), entry.path().filename().string());
        }
    }
    return 0;
}