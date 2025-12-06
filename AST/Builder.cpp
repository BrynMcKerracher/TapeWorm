/**
 * @file Builder.cpp
 * @author brynm
 */
#include "Builder.h"

#include <iostream>

#include "AtomicNode.h"

namespace TapeWorm::AST {
     Global Builder::BuildAST(const std::vector<InterWorm::Token> &tokens) {
        tokenStream = tokens;
        Global globalNode = std::make_unique<GlobalNode>();
        current = 0;

        while (!AtEndOfStream()) {
            globalNode->subNodes.emplace_back(BuildNextNode());
        }

        return std::move(globalNode);
    }

    Node Builder::BuildNextNode() {
        if (Match({InterWorm::Token::Type::JumpIfZero})) {
            Block block = std::make_unique<BlockNode>();
            if (!blockNodes.empty()) blockNodes.top()->isLeaf = false;
            blockNodes.push(block.get());
            while (Peek().type != InterWorm::Token::Type::JumpNotZero) {
                Node expr = BuildNextNode();

                if (AtEndOfStream()) {
                    throw std::exception("Parse Error: Unmatched '['\n");
                }

                block->subNodes.emplace_back(std::move(expr));
            }
            Consume(InterWorm::Token::Type::JumpNotZero);
            blockNodes.pop();
            UpdateStatementStatus(block);
            return block;
        }
        Advance();
        return std::make_unique<AtomicNode>(Previous());
    }

    bool Builder::Match(const std::initializer_list<InterWorm::Token::Type> types) {
        for (const InterWorm::Token::Type token : types) {
            if (Check(token)) {
                Advance();
                return true;
            }
        }
        return false;
    }

    bool Builder::Check(const InterWorm::Token::Type type) const {
        if (AtEndOfStream()) return false;
        return Peek().type == type;
    }

    bool Builder::AtEndOfStream() const {
        return Peek().type == InterWorm::Token::Type::EndOfFile;
    }

    InterWorm::Token Builder::Advance() {
        if (!AtEndOfStream()) current++;
        return Previous();
    }

    InterWorm::Token Builder::Consume(const InterWorm::Token::Type type) {
        if (Check(type)) return Advance();
        throw std::exception("Parse Error!");
    }

    const InterWorm::Token& Builder::Peek() const {
        return tokenStream[current];
    }

    const InterWorm::Token& Builder::Previous() const {
        return tokenStream[current - 1];
    }

    void Builder::UpdateStatementStatus(const Block &block) {
        block->isStatement = IsStatement(block.get());
    }

    bool Builder::IsStatement(const BlockNode *blockNode) {
         int64_t pointerOffset = 0;
         for (const Node& node : blockNode->subNodes) {
             const auto [type, length] = reinterpret_cast<AtomicNode*>(node.get())->token;
             if (type == InterWorm::Token::IncPointer) pointerOffset += length;
             else if (type == InterWorm::Token::DecPointer) pointerOffset -= length;
         }
         return pointerOffset == 0;
     }
}