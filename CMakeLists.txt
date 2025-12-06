cmake_minimum_required(VERSION 4.0)
project(TapeWorm CXX)

set(CMAKE_CXX_STANDARD 26)

# Set Boost variables
set(Boost_DIR "C:\\Boost\\lib\\cmake\\Boost-1.89.0")
find_package(Boost 1.89.0 REQUIRED)

# Set AsmJit variables
set(ASMJIT_DIR "asmjit-master")
set(ASMJIT_STATIC TRUE)
add_subdirectory("${ASMJIT_DIR}")

add_executable(TapeWorm
        main.cpp
        Util.cpp
        Util.h
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
        AST/OptimiserVisitor.cpp
        AST/OptimiserVisitor.h
        ScannerOptimiser.cpp
        ScannerOptimiser.h
)

target_link_libraries(TapeWorm Boost::headers)
target_link_libraries(TapeWorm asmjit::asmjit)