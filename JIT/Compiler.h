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
#include <ostream>

namespace TapeWorm::JIT {
    using MainEntry = void(*)(uintptr_t);

    class Compiler {
    public:
        void Compile(const std::vector<InterWorm::Op::Type>& interwormStream);

        static std::ostream* output;// = &std::cout;
    private:
        asmjit::JitRuntime runtime;
        std::vector<uint8_t> runtimeMemory;
        constexpr static std::size_t RuntimeMemorySize = 80000;

        struct ControlFlowPair {
            ControlFlowPair(const asmjit::Label& open, const asmjit::Label& close) :
                open(open), close(close)
            {}

            asmjit::Label open;
            asmjit::Label close;
        };

        static void WriteCharacter(uint8_t character);
        static uint8_t ReadCharacter();
    };
}

#endif