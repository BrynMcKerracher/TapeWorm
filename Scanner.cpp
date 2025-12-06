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

        for (current = 0; !AtEndOfSource(); ++current) {
            start = current;
            switch (sourceCode[current]) {
                case '>': tokens.push_back(MakeConsecutiveToken(Token::Type::IncPointer, '>')); break;
                case '<': tokens.push_back(MakeConsecutiveToken(Token::Type::DecPointer, '<')); break;
                case '+': tokens.push_back(MakeConsecutiveToken(Token::Type::IncCell, '+')); break;
                case '-': tokens.push_back(MakeConsecutiveToken(Token::Type::DecCell, '-')); break;
                case '.': tokens.push_back(Token {Token::OutputCell, 1}); break;
                case ',': tokens.push_back(Token {Token::InputCell, 1}); break;
                case '[': tokens.push_back(MakeOpenControlFlowToken()); break;
                case ']': tokens.push_back(Token {Token::JumpNotZero, 1}); break;
                default: break;
            }
            if (tokens.size() > 1) {
                if (tokens.back().type == tokens[tokens.size() - 2].type) {
                    std::cout << "Double up! " << tokens.back().type << " \n";
                }
            }
        }
        tokens.push_back(Token {Token::Type::EndOfFile, 0});
        return tokens;
    }

    Token Scanner::MakeConsecutiveToken(const Token::Type type, const char c) {
        while (Match(c)) {}
        return {type, current - start};
    }

    Token Scanner::MakeOpenControlFlowToken() {
        if (Match('+', ']') or Match('-', ']')) {
            return {Token::Type::ClearCell, 1};
        }
        return {Token::Type::JumpIfZero, current - start};
    }

    bool Scanner::AtEndOfSource() const {
        return current >= sourceCode.size();
    }

    bool Scanner::Match(const char expected) {
        if (AtEndOfSource()) return false;
        if (sourceCode[current] != expected) return false;

        current++;
        return true;
    }

    bool Scanner::Match(const char a, const char b) {
        if (current + 1 >= sourceCode.size()) return false;
        if (sourceCode[current] != a or sourceCode[current + 1] != b) return false;

        current += 2;
        return true;
    }
}
