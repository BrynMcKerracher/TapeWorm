cmake_minimum_required(VERSION 4.0)
project(TapeWorm CXX)

set(CMAKE_CXX_STANDARD 23 REQUIRED)

# Set Boost variables
# set(Boost_DIR "C:\\Boost\\lib\\cmake\\Boost-1.89.0")
# find_package(Boost 1.89.0 REQUIRED)

# Set AsmJit variables
set(ASMJIT_DIR "asmjit-master")
set(ASMJIT_STATIC TRUE)
add_subdirectory("${ASMJIT_DIR}")

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
)

# target_link_libraries(TapeWorm Boost::headers)
target_link_libraries(TapeWorm asmjit::asmjit)