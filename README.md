# TapeWorm: A JIT Brainfuck Interpreter

![GitHub Release](https://img.shields.io/github/v/release/BrynMcKerracher/TapeWorm?link=https%3A%2F%2Fgithub.com%2FBrynMcKerracher%2FTapeWorm%2Freleases%2F)
[![Linux Build Tests (Full Pipeline)](https://github.com/BrynMcKerracher/TapeWorm/actions/workflows/Linux%20Build%20Tests%20(Full%20Pipeline).yml/badge.svg)](https://github.com/BrynMcKerracher/TapeWorm/actions/workflows/Linux%20Build%20Tests%20(Full%20Pipeline).yml)
[![Windows Build Tests (Full Pipeline)](https://github.com/BrynMcKerracher/TapeWorm/actions/workflows/Windows%20Build%20Tests%20(Full%20Pipeline).yml/badge.svg)](https://github.com/BrynMcKerracher/TapeWorm/actions/workflows/Windows%20Build%20Tests%20(Full%20Pipeline).yml)

TapeWorm is a [brainfuck](https://en.wikipedia.org/wiki/Brainfuck) interpreter for Windows and Linux `x86-64` platforms, written in C++ and `x86` assembly. 

## Features
- Fast assembly-based JIT framework.
- Multiple optimisation passes for better runtime performance.
- CLI interface for directly running brainfuck files.
- Continually tested to ensure stability on target platforms.
- Works out of the box without additional configuration.

## Installation
### On Windows
  **Using The Installer (Easiest)**
  - Go to the [latest release](https://github.com/BrynMcKerracher/TapeWorm/releases/latest) then download and run the Windows installer file.
  - This is the preferred method because it associates `.b` files with TapeWorm, allowing you to run brainfuck files by double-clicking them.
  
  **Download The Binaries:**
  - Go to the [latest release](https://github.com/BrynMcKerracher/TapeWorm/releases/latest) and download the Windows executable.
  - <mark>Note</mark>: This will not associate `.b` files, and in some cases anti-virus software may incorrectly suppress execution.

  **Build From Source:**
  - Go to the [latest release](https://github.com/BrynMcKerracher/TapeWorm/releases/latest) and download source `.zip` file.
  - [Build](https://cmake.org/cmake/help/book/mastering-cmake/chapter/Getting%20Started.html) the source using [CMake](https://cmake.org/).

### On Linux
  **Download The Binaries:**
  - Go to the [latest release](https://github.com/BrynMcKerracher/TapeWorm/releases/latest) and download the Linux binary.

  **Build From Source:**
  - Go to the [latest release](https://github.com/BrynMcKerracher/TapeWorm/releases/latest) and download source `.zip` file.
  - [Build](https://cmake.org/cmake/help/book/mastering-cmake/chapter/Getting%20Started.html) the source using [CMake](https://cmake.org/).

## Usage
Invoke the TapeWorm executable in a CLI (such as `bash` or `command prompt`) and pass it the path to your brainfuck source file:
### On Windows
```
TapeWorm.exe "path/to/file/source.b"
```
If you installed TapeWorm using the Windows Installer, `.b` are associated with TapeWorm, so you can double-click them to run.
### On Linux
```
./TapeWorm "path/to/file/source.b"
```
The brainfuck program will print and receive input through the same CLI.

### Command Line Arguments
You can change the behaviour of TapeWorm by passing in any of the following arguments.
|     Argument     |                 Effect                |
|:----------------:|---------------------------------------|
| `--no-opt`       | Disable all optimisation passes.      |
| `--no-ast-opt`   | Disable AST optimisation pass.        |
| `--no-ir-opt`    | Disable IR optimisation pass.         |
| `--help`         | Displays version and usage info.      |

## Documentation
- [Source Code Documentation](https://brynmckerracher.github.io/TapeWorm/)
- [Roadmap](https://github.com/users/BrynMcKerracher/projects/11)
- [Project Health](https://github.com/users/BrynMcKerracher/projects/11/views/1?pane=info&statusUpdateId=272113)
