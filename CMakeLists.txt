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
        AST/TreeBuilder.cpp
        AST/TreeBuilder.h
        AST/BlockNode.cpp
        AST/BlockNode.h
        AST/GlobalNode.cpp
        AST/GlobalNode.h
        AST/NodeVisitor.h
        AST/AtomicNode.cpp
        AST/AtomicNode.h
        AST/CompilerVisitor.cpp
        AST/CompilerVisitor.h
        Debug/ASTAnalyser.cpp
        Debug/ASTAnalyser.h
        ScannerOptimiser.cpp
        ScannerOptimiser.h
        AST/LeafStatementNode.cpp
        AST/LeafStatementNode.h
        AST/TreeOptimiser.cpp
        AST/NodeEditor.cpp
)

target_link_libraries(TapeWorm asmjit::asmjit)