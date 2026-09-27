cmake_minimum_required(VERSION 3.28)
project(TapeWormTests CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(BUILD_SHARED_LIBS OFF)
#set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

set(ASMJIT_STATIC TRUE)
include(FetchContent)
FetchContent_Declare(
        asmjit
        GIT_REPOSITORY https://github.com/asmjit/asmjit.git
)
FetchContent_MakeAvailable(asmjit)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        DOWNLOAD_EXTRACT_TIMESTAMP true
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(TapeWormTests
        FullPipelineTest.cpp
        ../Util.cpp
        ../Util.h
        ../Token.cpp
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
        ../AST/MetaTerminalNode.cpp
        ../AST/MetaTerminalNode.h
        ../AST/TreeOptimiser.cpp
        ../AST/NodeTypes.h
        ../Debug/Analyser.cpp
        ../Debug/Analyser.h
        ../ScannerOptimiser.cpp
        ../ScannerOptimiser.h
)

target_link_libraries(TapeWormTests PRIVATE asmjit::asmjit GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(TapeWormTests)
