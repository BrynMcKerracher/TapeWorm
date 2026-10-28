#ifndef TAPEWORMTESTS_VIRTUALTAPE_H
#define TAPEWORMTESTS_VIRTUALTAPE_H

#include "RegisterContext.h"
#include "VirtualCell.h"
#include <asmjit/x86.h>
#include <map>

namespace TapeWorm::JIT {
    class VirtualTape {
    public:
        explicit VirtualTape(x86::Assembler& _assembler);

        void IncPointer();
        void DecPointer();
        void AddPointer(int32_t offset);
        void SubPointer(int32_t offset);

        void Inc();
        void Dec();
        void Add(int32_t offset);
        void Sub(int32_t offset);
        void Multiply(int32_t factor);
        void Flush();
        void FlushPointer();
        void FlushCell();
        void ClearCell();

        static void writeCharacter(Cell character);
        static Cell readCharacter();
    private:
        std::map<int32_t, VirtualCell> virtualMemory;
        x86::Assembler& assembler;
        RegisterContext registers;

        int32_t pointerAggregate = 0;
    };
} // TapeWorm::JIT

#endif //TAPEWORMTESTS_VIRTUALTAPE_H
