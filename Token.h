/**
 * @file Token.h
 * @author brynm
 */
#ifndef TAPEWORM_TOKEN_H
#define TAPEWORM_TOKEN_H

#include <string>

namespace TapeWorm::InterWorm {
    struct Token {
        enum Type {
            None,
            IncPointer,
            DecPointer,
            IncCell,
            DecCell,
            ClearCell,
            InputCell,
            OutputCell,
            JumpIfZero,
            JumpNotZero,
            Move,
            Difference,
            EndOfFile
        };

        Type type = None;
        int64_t length = 0;

        [[nodiscard]] std::string ToString() const;
        static std::string ToString(const Token& token);
    };
}

#endif