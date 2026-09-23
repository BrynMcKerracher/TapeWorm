/**
 * @file TreeBuilder.h
 * @author Bryn McKerracher
 */
#ifndef TAPEWORM_BUILDER_H
#define TAPEWORM_BUILDER_H

#include "NodeTypes.h"
#include "../Token.h"
#include <stack>

namespace TapeWorm::AST {
    class Builder {
    public:
        Global BuildAST(const std::vector<InterWorm::Token>& tokens);
    private:
        std::size_t currentTokenIndex = 0;
        std::size_t currentSyntacticDepth = 0;
        std::vector<InterWorm::Token> inputTokens;

        [[nodiscard]] Global BuildGlobalNode();
        [[nodiscard]] Terminal BuildTerminalNode() const;
        [[nodiscard]] Syntactic BuildSyntacticNode();
        [[nodiscard]] Node BuildNextNode();

        [[nodiscard]] bool Match(InterWorm::Token::Type token);
        [[nodiscard]] bool Check(InterWorm::Token::Type type) const;
        [[nodiscard]] bool AtEndOfStream() const;
        InterWorm::Token Advance();
        [[nodiscard]] const InterWorm::Token& Current() const;
        [[nodiscard]] const InterWorm::Token& Previous() const;
    };
} // TapeWorm

#endif //TAPEWORM_BUILDER_H