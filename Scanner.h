/**
 * @file Scanner.h
 * @author Bryn McKerracher
 */
#ifndef TAPEWORM_SCANNER_H
#define TAPEWORM_SCANNER_H

#include <vector>
#include <string>
#include "Token.h"

namespace TapeWorm::InterWorm {
    class Scanner {
    public:
        [[nodiscard]] std::vector<Token> Scan(const std::string& source);
    private:
        int64_t current = 0;
        int64_t start = 0;
        std::string sourceCode;
        std::vector<Token> tokens;

        [[nodiscard]] Token MakeConsecutiveToken(Token::Type type, char c);
        [[nodiscard]] bool Match(char expected);
    };
}

#endif