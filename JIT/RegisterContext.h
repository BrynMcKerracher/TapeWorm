#ifndef TAPEWORMTESTS_REGISTERCONTEXT_H
#define TAPEWORMTESTS_REGISTERCONTEXT_H

#include <asmjit/asmjit.h>
#include <asmjit/x86.h>
#include <vector>

namespace TapeWorm::JIT {
    using namespace asmjit;

    class RegisterContext {
    public:
        explicit RegisterContext(x86::Assembler& _assembler);

        x86::Gp& CellPointer();
        x86::Gp& ScalarRegister(uint8_t index);
        x86::Vec& VectorRegister(uint8_t index);
        x86::Gp& CallRegister(uint8_t index);
    private:
        std::vector<x86::Gp> scalarRegisters;
        std::vector<x86::Vec> vectorRegisters;
        std::vector<x86::Gp> callRegisters;
        x86::Assembler& assembler;
    };
} // TapeWorm

#endif //TAPEWORMTESTS_REGISTERCONTEXT_H
