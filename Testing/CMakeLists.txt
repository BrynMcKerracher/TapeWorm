cmake_minimum_required(VERSION 3.28)
project(TapeWormTests CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

set(ASMJIT_STATIC TRUE)
include(FetchContent)
FetchContent_Declare(
        asmjit
        GIT_REPOSITORY https://github.com/asmjit/asmjit.git
)
FetchContent_MakeAvailable(asmjit)

add_executable(TapeWormTests
        main.cpp
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

target_link_libraries(TapeWormTests asmjit::asmjit)

add_test(NAME HelloWorld COMMAND TapeWormTests "../Tests/hw.b")
set_property(TEST HelloWorld PROPERTY PASS_REGULAR_EXPRESSION "11208743478092974376")

add_test(NAME Mandelbrot COMMAND TapeWormTests "../Tests/mandelbrot.b")
set_property(TEST Mandelbrot PROPERTY PASS_REGULAR_EXPRESSION "4686367950102506177")

add_test(NAME TortureTest COMMAND TapeWormTests "../Tests/bitwidth.b")
set_property(TEST TortureTest PROPERTY PASS_REGULAR_EXPRESSION "5756710207797096192")