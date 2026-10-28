#include "RegisterContext.h"

namespace TapeWorm::JIT {
    RegisterContext::RegisterContext(x86::Assembler& _assembler) : assembler(_assembler) {
        scalarRegisters = {
            x86::r15,
            x86::r14,
            x86::r13,
            x86::r12,
        };
        vectorRegisters = {
            x86::xmm0,
            x86::xmm1,
        };

#if defined (_WIN32) || defined (_WIN64) || defined (__CYGWIN__)
        callRegisters = {
            x86::rcx,
            x86::rdx,
            x86::r8,
            x86::r9,
        };
#else
        callRegisters = {
            x86::rdi,
            x86::rsi,
            x86::rdx,
            x86::rcx,
            x86::r8,
            x86::r9,
        };
#endif
    }

    x86::Gp& RegisterContext::CellPointer() {
        return scalarRegisters[0];
    }

    x86::Gp& RegisterContext::ScalarRegister(const uint8_t index) {
        return scalarRegisters[index];
    }

    x86::Vec& RegisterContext::VectorRegister(const uint8_t index) {
        return vectorRegisters[index];
    }

    x86::Gp& RegisterContext::CallRegister(const uint8_t index) {
        return callRegisters[index];
    }
} // TapeWorm