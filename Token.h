/**
 * @file Token.h
 * @author Bryn McKerracher
 */
#ifndef TAPEWORM_TOKEN_H
#define TAPEWORM_TOKEN_H

#include <string>
#include <cstdint>

namespace TapeWorm::InterWorm {
    /**
     * @struct Token
     * @brief Token-based intermediate language which is reducible to vanilla brainfuck.
     *
     * InterWorm::Token (I::T) is an intermediate language for the first phase of the TapeWorm interpretation pipeline.
     * I::T is a language that contains more operations than vanilla brainfuck but is immediately reducible to vanilla brainfuck
     * sequences.
     **/
    struct Token {
        enum Type {
            Nop,
            IncPointer,  //Equivalent to brainfuck '>'
            DecPointer,  //Equivalent to brainfuck '<'
            IncCell,     //Equivalent to brainfuck '+'
            DecCell,     //Equivalent to brainfuck '-'
            ClearCell,
            InputCell,   //Equivalent to brainfuck ','
            OutputCell,  //Equivalent to brainfuck '.'
            JumpIfZero,  //Equivalent to brainfuck '['
            JumpNotZero, //Equivalent to brainfuck ']'
            Move,
            Difference,
            EndOfFile
        };

        /**
         * @name type
         * @brief Tagged type.
         **/
        Type type = Nop;

        int64_t length = 0;

        [[nodiscard]] std::string ToString() const;
        static std::string ToString(const Token& token);
    };
}

#endif