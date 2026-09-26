cmake_minimum_required(VERSION 3.28)
project(TapeWorm CXX)

set(CMAKE_CXX_STANDARD 17)
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

add_executable(TapeWorm
        main.cpp
        Util.cpp
        Util.h
        Token.cpp
        Token.h
        Scanner.cpp
        Scanner.h
        Op.cpp
        Op.h
        JIT/Compiler.cpp
        JIT/Compiler.h
        AST/NodeBase.h
        AST/Builder.cpp
        AST/Builder.h
        AST/SyntacticNode.cpp
        AST/SyntacticNode.h
        AST/GlobalNode.cpp
        AST/GlobalNode.h
        AST/NodeReadOnlyVisitor.h
        AST/TerminalNode.cpp
        AST/TerminalNode.h
        AST/CompilerVisitor.cpp
        AST/CompilerVisitor.h
        AST/MetaTerminalNode.cpp
        AST/MetaTerminalNode.h
        AST/TreeOptimiser.cpp
        AST/NodeTypes.h
        Debug/Analyser.cpp
        Debug/Analyser.h
        ScannerOptimiser.cpp
        ScannerOptimiser.h
)

target_link_libraries(TapeWorm asmjit::asmjit)