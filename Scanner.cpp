/**
 * @file Scanner.cpp
 * @author brynm
 */
#include "Scanner.h"

#include <iostream>

namespace TapeWorm::InterWorm {
    std::vector<Token> Scanner::Scan(const std::string &source) {
        start = current = 0;
        sourceCode = source;
        tokens.clear();
        tokens.reserve(source.size());

        for (current = 0; current < sourceCode.size(); ++current) {
            start = current;
            switch (sourceCode[current]) {
                case '>': tokens.push_back(MakeConsecutiveToken(Token::Type::IncPointer, '>')); break;
                case '<': tokens.push_back(MakeConsecutiveToken(Token::Type::DecPointer, '<')); break;
                case '+': tokens.push_back(MakeConsecutiveToken(Token::Type::IncCell, '+')); break;
                case '-': tokens.push_back(MakeConsecutiveToken(Token::Type::DecCell, '-')); break;
                case '.': tokens.push_back({Token::OutputCell, 1}); break;
                case ',': tokens.push_back({Token::InputCell, 1}); break;
                case '[': tokens.push_back({Token::JumpIfZero, 1}); break;
                case ']': tokens.push_back({Token::JumpNotZero, 1}); break;
                default: break;
            }
        }
        return tokens;
    }

    Token Scanner::MakeConsecutiveToken(const Token::Type type, const char c) {
        while (Match(c)) {}
        current--;
        return {type, current - start + 1};
    }

    bool Scanner::Match(const char expected) {
        if (current >= sourceCode.size()) return false;
        if (sourceCode[current] != expected) return false;

        current++;
        return true;
    }
}
