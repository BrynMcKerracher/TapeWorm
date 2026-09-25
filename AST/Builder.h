/**
 * @file Builder.h
 * @author Bryn McKerracher
 **/
#ifndef TAPEWORM_BUILDER_H
#define TAPEWORM_BUILDER_H

#include "NodeTypes.h"
#include "../Token.h"

namespace TapeWorm::AST {
    /**
     * @class Builder
     * @brief Builds an AST from an array of InterWorm::Token.
     *
     * This is the first pass in the AST phase of the pipeline.
     **/
    class Builder {
    public:
        /**
         * @brief Creates an AST from an array of tokens.
         * @param tokens A list of tokens.
         * @return A Global representing the root node of the AST.
         *
         * Validation and grammar checking is not performed in this phase.
         */
        Global BuildAST(const std::vector<InterWorm::Token>& tokens);
    private:
        /**
         * @name currentTokenIndex
         * @brief Represents the index in the token array this builder is currently looking at.
         */
        std::size_t currentTokenIndex = 0;

        /**
         * @name currentSyntacticDepth
         * @brief The current number of Syntactic nodes we're in (recursively).
         */
        std::size_t currentSyntacticDepth = 0;

        /**
         * @name inputTokens
         * @brief The array of tokens from which we are building the AST.
         *
         * This field is used to store a copy of the token list arg provided by the BuildAST() method.
         */
        std::vector<InterWorm::Token> inputTokens;

        [[nodiscard]] Global BuildGlobalNode();
        [[nodiscard]] Terminal BuildTerminalNode() const;
        [[nodiscard]] Syntactic BuildSyntacticNode();
        [[nodiscard]] Node BuildNextNode();

        [[nodiscard]] bool Match(InterWorm::Token::Type token);
        [[nodiscard]] bool Check(InterWorm::Token::Type type) const;
        [[nodiscard]] bool AtEndOfTokens() const;
        InterWorm::Token Advance();
        [[nodiscard]] const InterWorm::Token& Current() const;
        [[nodiscard]] const InterWorm::Token& Previous() const;
    };
} // TapeWorm

#endif //TAPEWORM_BUILDER_H