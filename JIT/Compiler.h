/**
 * @file Compiler.h
 * @author Bryn McKerracher
 */
#ifndef TAPEWORM_JITCOMPILER_H
#define TAPEWORM_JITCOMPILER_H

#include "../Op.h"
#include "asmjit/core/jit_runtime.h"
#include "asmjit/x86/x86_assembler.h"

#include <vector>
#include <iostream>

namespace TapeWorm::JIT {
    using MainEntry = void(*)(uintptr_t);
    using LabelPair = std::pair<asmjit::Label, asmjit::Label>;

    class Compiler {
    public:
        void Compile(const std::vector<InterWorm::Op::Type>& interwormStream);
        ~Compiler();
    private:
        constexpr static std::size_t RuntimeMemorySize = 30000;
        asmjit::JitRuntime runtime;
        asmjit::CodeHolder code;
        asmjit::x86::Assembler assembler;
        std::vector<uint8_t> runtimeMemory;
        MainEntry mainEntry = nullptr;

        static void WriteCharacter(uint8_t character);
        static uint8_t ReadCharacter();
    };
}

#endif