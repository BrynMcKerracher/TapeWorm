cmake_minimum_required(VERSION 3.28)
project(TapeWormTests CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT ${APPLE})
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
endif()

set(ASMJIT_STATIC TRUE)
include(FetchContent)
FetchContent_Declare(
        asmjit
        GIT_REPOSITORY https://github.com/asmjit/asmjit.git
)
FetchContent_MakeAvailable(asmjit)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.8.1 # or a later release
)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

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
        ../Debug/InstructionCoverage.cpp
        ../Debug/InstructionCoverage.h
        TestEnvironmentSetup.cpp
        TestEnvironmentSetup.h
        ../CLI/OptionsManagement.cpp
        ../CLI/OptionsManagement.h
)

target_link_libraries(TapeWormTests PRIVATE asmjit::asmjit Catch2::Catch2WithMain)
catch_discover_tests(TapeWormTests)
