/**
 * @file Builder.h
 * @author brynm
 */
#ifndef TAPEWORM_BUILDER_H
#define TAPEWORM_BUILDER_H

#include "GlobalNode.h"
#include "BlockNode.h"
#include "NodeVisitor.h"
#include "../Token.h"

#include <stack>

namespace TapeWorm::AST {
    class Builder {
    public:
        Global BuildAST(const std::vector<InterWorm::Token>& tokens);
    private:
        std::size_t current = 0;
        std::vector<InterWorm::Token> tokenStream;
        std::stack<BlockNode*> blockNodes;

        Node BuildNextNode();

        bool Match(std::initializer_list<InterWorm::Token::Type> types);
        [[nodiscard]] bool Check(InterWorm::Token::Type type) const;
        [[nodiscard]] bool AtEndOfStream() const;
        InterWorm::Token Advance();
        InterWorm::Token Consume(InterWorm::Token::Type type);
        [[nodiscard]] const InterWorm::Token& Peek() const;
        [[nodiscard]] const InterWorm::Token& Previous() const;

        void UpdateParentLeafStatus(const Block& block);
        static void UpdateStatementStatus(const Block& block);
        static bool IsStatement(const BlockNode *blockNode);
    };
} // TapeWorm

#endif //TAPEWORM_BUILDER_H