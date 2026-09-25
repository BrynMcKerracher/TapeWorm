cmake_minimum_required(VERSION 4.0)
project(TapeWormTests CXX)

set(CMAKE_CXX_STANDARD 26)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        DOWNLOAD_EXTRACT_TIMESTAMP true
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Set Boost variables
# set(Boost_DIR "C:\\Boost\\lib\\cmake\\Boost-1.89.0")
# find_package(Boost 1.89.0 REQUIRED)

# Set AsmJit variables
set(ASMJIT_DIR "asmjit-master")
set(ASMJIT_STATIC TRUE)
add_subdirectory("${ASMJIT_DIR}")

add_executable(TapeWormTests
        TapeWormTests.cpp
        ../Util.cpp
        ../Util.h
        ../Token.h
        ../Scanner.cpp
        ../Scanner.h
        ../Op.cpp
        ../Op.h
        ../JIT/Compiler.cpp
        ../JIT/Compiler.h
        ../AST/NodeBase.h
        ../AST/Builder.cpp
        ../AST/Builder.h
        ../AST/SyntacticNode.cpp
        ../AST/SyntacticNode.h
        ../AST/GlobalNode.cpp
        ../AST/GlobalNode.h
        ../AST/NodeReadOnlyVisitor.h
        ../AST/TerminalNode.cpp
        ../AST/TerminalNode.h
        ../AST/CompilerVisitor.cpp
        ../AST/CompilerVisitor.h
        ../Debug/ASTAnalyser.cpp
        ../Debug/ASTAnalyser.h
        ../ScannerOptimiser.cpp
        ../ScannerOptimiser.h
        ../Token.cpp
        ../AST/MetaTerminalNode.cpp
        ../AST/MetaTerminalNode.h
        FullPipelineTests.cpp
        ../Debug/Profiler.cpp
        ../Debug/Profiler.h
        ../AST/TreeOptimiser.cpp
        ../AST/TreeOptimiser.h
        ../AST/NodeReadWriteVisitor.h
)

# target_link_libraries(TapeWormTests Boost::headers)
target_link_libraries(TapeWormTests asmjit::asmjit)
target_link_libraries(TapeWormTests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(TapeWormTests)