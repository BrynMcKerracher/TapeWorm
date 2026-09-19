# TapeWorm: A JIT Brainfuck Compiler
TapeWorm is an optimising multipass [brainfuck](https://en.wikipedia.org/wiki/Brainfuck) interpreter for `x86` platforms, written in C++.

It produces and executes handwritten `x86` assembly using [asmjit](https://github.com/asmjit/asmjit) as a runtime assembler. 

## Goals
The main goal of TapeWorm is to speed up runtime execution of brainfuck, especially long complex sequences such as fractal generation. 

Secondary to this is to over-engineer a compiler with as many features as possible to create a rich suite of options for those brave or foolhardy enough to work in vanilla brainfuck. 

Note: No AI was used at any point during development.

## Installation
### Pre-compiled Binaries
Pre-compiled binaries for both 
Windows and Linux exist under the [releases](https://github.com/BrynMcKerracher/TapeWorm/releases) tab. It's recommended you use the latest release. 

### Manual
You can also build the project from source using `Make`/`CMake` in the [usual way](https://cmake.org/cmake/help/book/mastering-cmake/chapter/Getting%20Started.html).

## Usage
Invoke the TapeWorm executable in a CLI and pass it the path to your brainfuck source file.

For example:
```
./TapeWorm "path/to/file/source.b"
```
At the moment TapeWorm only supports single-file interpretation, but more complex compilation modes are planned for the future.

## Further Info
### Compilation Pipeline
TapeWorm has a 4-phase pipeline (and another 2 optional optimisation phases) from brainfuck source code to `x86` assembly. 
#### 1. Tokenisation 
Produces a sequence of value tokens that are a bit more expressive than the standard 8 brainfuck commands.
#### 2. AST Generation
Generates an abstract syntax tree from the token sequence.
#### 3. Intermediate Language Generation
Creates a sequence of instructions in TapeWorm's intermediate language _InterWorm_.
#### 4. Assembly Generation
Lastly _InterWorm_ is jitted into `x86` assembly and executed.

### InterWorm ISA
Documentation for the _InterWorm_ ISA is still in the works and will be made available on the project wiki at a later date.
