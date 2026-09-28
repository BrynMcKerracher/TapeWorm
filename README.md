# TapeWorm: A JIT Brainfuck Compiler

[![Ubuntu 22.04 Full Pipeline Tests](https://github.com/BrynMcKerracher/TapeWorm/actions/workflows/Ubuntu-22.04%20Tests.yml/badge.svg)](https://github.com/BrynMcKerracher/TapeWorm/actions/workflows/Ubuntu-22.04%20Tests.yml) [![Windows 2025 Server Full Pipeline Tests](https://github.com/BrynMcKerracher/TapeWorm/actions/workflows/Windows-2025-Server-Full-Pipeline-Tests.yml/badge.svg)](https://github.com/BrynMcKerracher/TapeWorm/actions/workflows/Windows-2025-Server-Full-Pipeline-Tests.yml) [![Ubuntu 24.04 Full Pipeline Tests](https://github.com/BrynMcKerracher/TapeWorm/actions/workflows/Ubuntu-24.04-Full-Pipeline-Tests.yml/badge.svg)](https://github.com/BrynMcKerracher/TapeWorm/actions/workflows/Ubuntu-24.04-Full-Pipeline-Tests.yml)

TapeWorm is an optimising multipass [brainfuck](https://en.wikipedia.org/wiki/Brainfuck) interpreter for `x86_64` platforms, written in C++. It provides a CLI interface for running vanilla brainfuck source files.

No AI was used at any point during development.

## Goals
**1.** Optimise runtime execution of brainfuck. To this end it produces and executes optimised `x86_64` assembly using [asmjit](https://github.com/asmjit/asmjit) as a runtime assembler. 

**2.** Create a feature-rich interpreter to make working in vanilla brainfuck as painless as possible.

## Installation
#### Pre-compiled Binaries
Pre-compiled binaries for both 
Windows and Linux can be found under the [releases](https://github.com/BrynMcKerracher/TapeWorm/releases) tab. 

#### Manual
You can also build the project from source using [CMake](https://cmake.org/) in the [usual way](https://cmake.org/cmake/help/book/mastering-cmake/chapter/Getting%20Started.html).

## Usage
Invoke the TapeWorm executable in a CLI and pass it the path to your brainfuck source file:
```
./TapeWorm "path/to/file/source.b"
```
