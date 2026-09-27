/**
 * @file Op.h
 * @author Bryn McKerracher
 */
#ifndef TAPEWORM_OP_H
#define TAPEWORM_OP_H

#include <cstdint>
#include <string>

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
            AddImmediateExtended,
            SubImmediate,
            SubImmediateExtended,
            AddPointer,
            AddPointerExtended,
            SubPointer,
            SubPointerExtended,
            Add,
            Subtract,
            NumOps
        };

        static std::string ToString(Type type);
        static std::size_t NumArgs(Type type);
    };
} // TapeWorm

#endif //TAPEWORM_OP_H