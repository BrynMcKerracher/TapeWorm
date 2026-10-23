/**
 * @file Scanner.cpp
 * @author Bryn McKerracher
 */
#include "Scanner.h"

#include <iostream>
#include <cstring>

namespace TapeWorm::InterWorm {
    std::vector<Token> Scanner::Scan(const std::string &source) {
        sourceCode = source;
        start = current = &sourceCode[0];
        tokens.clear();
        tokens.reserve(source.size());

        while (!AtEndOfSequence()) {
            start = current;
            tokens.emplace_back(NextToken());
        }
        return tokens;
    }

    Token Scanner::MakeConsecutiveToken(const Token::Type type, const char c) {
        while (Match(c)) {}
        return {type, current - start};
    }

    bool Scanner::Match(const char expected) {
        if (AtEndOfSequence()) return false;
        if (*current != expected) return false;
        current++;
        return true;
    }

    bool Scanner::Match(const std::string &expected) {
        if (current - start + expected.size() >= sourceCode.size()) return false;

        const char* originalCurrent = current;
        for (const char i : expected) {
            if (i != *current++) {
                current = originalCurrent;
                return false;
            }
        }
        return true;
    }

    bool Scanner::AtEndOfSequence() const {
        return *current == '\0';
    }

    Token Scanner::MakeToken(const Token::Type type) const {
        Token token;
        token.type = type;
        token.length = current - start;
        return token;
    }

    Token Scanner::NextToken() {
        if (AtEndOfSequence()) return MakeToken(Token::Type::EndOfFile);
        switch (Advance()) {
            /// Simple Consecutive Tokens
            case '>': return MakeConsecutiveToken(Token::Type::IncPointer, '>');
            case '<': return MakeConsecutiveToken(Token::Type::DecPointer, '<');
            case '+': return MakeConsecutiveToken(Token::Type::IncCell, '+');
            case '-': return MakeConsecutiveToken(Token::Type::DecCell, '-');
            case '.': return {Token::OutputCell, 1};
            case ',': return {Token::InputCell, 1};
            case '[': {
                if (Match("-]") or Match("+]")) return {Token::ClearCell, 1};
                return {Token::JumpIfZero, 1};
            }
            case ']': return {Token::JumpNotZero, 1};
            default:  return {Token::Type::Error, 1};
        }
    }

    char Scanner::Advance() {
        return *current++;
    }
}
