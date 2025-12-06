/**
 * @file Compiler.h
 * @author brynm
 */
#ifndef TAPEWORM_JITCOMPILER_H
#define TAPEWORM_JITCOMPILER_H

#include <vector>

#include "../Op.h"
#include "asmjit/core/jitruntime.h"
#include "asmjit/x86/x86assembler.h"

namespace TapeWorm::JIT {
    using MainEntry = void(*)(uintptr_t);

    class Compiler {
    public:
        void Compile(const std::vector<InterWorm::Op::Type>& interwormStream);
    private:
        asmjit::JitRuntime runtime;
        std::vector<uint8_t> runtimeMemory;
        constexpr static std::size_t RuntimeMemorySize = 30000;

        struct ControlFlowPair {
            ControlFlowPair(const asmjit::v1_21::Label& open, const asmjit::v1_21::Label& close) :
                open(open), close(close)
            {}

            asmjit::v1_21::Label open;
            asmjit::v1_21::Label close;
        };

        static void WriteCharacter(uint8_t character);
        static uint8_t ReadCharacter();
    };
}

#endif