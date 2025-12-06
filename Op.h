/**
 * @file Op.h
 * @author brynm
 */
#ifndef TAPEWORM_OP_H
#define TAPEWORM_OP_H

#include <cstdint>

namespace TapeWorm::InterWorm {
    struct Op {
        enum Type : uint8_t {
            Nop = 0,
            IncPointer,
            DecPointer,
            IncCell,
            DecCell,
            ClearCell,
            InputCell,
            OutputCell,
            JumpIfZero,
            JumpNotZero,
            AddImmediate,
            SubImmediate,
            AddPointer,
            SubPointer,
            Add,
            Subtract,
            Multiply,
            LoadPointer,
        };
    };
} // TapeWorm

#endif //TAPEWORM_OP_H