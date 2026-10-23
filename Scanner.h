/**
 * @file Scanner.h
 * @author Bryn McKerracher
 */
#ifndef TAPEWORM_SCANNER_H
#define TAPEWORM_SCANNER_H

#include "Token.h"

#include <vector>
#include <string>

namespace TapeWorm::InterWorm {
    /**
     * @class Scanner
     * @brief A class for scanning text and transforming it into InterWorm tokens.
     **/
    class Scanner {
    public:
        /**
         * @brief Scans a brainfuck file and returns it as a list of InterWorm tokens.
         * @param source The file to be scanned.
         *
         * @return A vector of InterWorm tokens to be further analysed/compiled.
         **/
        [[nodiscard]] std::vector<Token> Scan(const std::string& source);
    private:
        const char* current = nullptr; ///Index of current character being analysed in a file.
        const char* start = nullptr;   ///Index of the beginning of the current token sequence in a file.
        std::string sourceCode;  ///Copy of the file's contents.
        std::vector<Token> tokens; ///List of tokens constructed from the given file.

        /**
         * @brief Creates an InterWorm token representing a single repeated operation.
         * @param type The type of the new token.
         * @param c The character to match against.
         * @return An InterWorm token representing a single repeating operation.
         **/
        [[nodiscard]] Token MakeConsecutiveToken(Token::Type type, char c);

        /**
         * @brief Consumes the next character if it matches the given parameter.
         * @param expected The character to look for.
         * @return True if the next character matched the expected character, otherwise false.
         **/
        [[nodiscard]] bool Match(char expected);

        [[nodiscard]] bool Match(const std::string& expected);

        [[nodiscard]] bool AtEndOfSequence() const;

        [[nodiscard]] Token MakeToken(Token::Type type) const;
        [[nodiscard]] Token NextToken();

        char Advance();
    };
}

#endif