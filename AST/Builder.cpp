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
        tokenStream.pop_back();
        blockNodes = {};
        Global globalNode = std::make_unique<GlobalNode>();
        current = 0;

        while (current < tokenStream.size()) {
            globalNode->subNodes.emplace_back(BuildNextNode());
        }

        return globalNode;
    }

    Node Builder::BuildNextNode() {
        Node node = nullptr;
        if (Match(InterWorm::Token::Type::JumpIfZero)) {
            node = BuildBlockNode();
        }
        else node = std::make_unique<AtomicNode>(Current());
        Advance();
        return node;
    }

    Block Builder::BuildBlockNode() {
         Block block = std::make_unique<BlockNode>();
         while (Current().type != InterWorm::Token::JumpNotZero) {
             block->subNodes.push_back(BuildNextNode());
         }
         return block;
    }

    bool Builder::Match(const InterWorm::Token::Type token) {
        if (Check(token)) {
            Advance();
            return true;
        }
        return false;
    }

    bool Builder::Check(const InterWorm::Token::Type type) const {
        if (AtEndOfStream()) return false;
        return Current().type == type;
    }

    bool Builder::AtEndOfStream() const {
        return Current().type == InterWorm::Token::Type::EndOfFile;
    }

    InterWorm::Token Builder::Advance() {
        if (!AtEndOfStream()) current++;
        return Previous();
    }

    InterWorm::Token Builder::Consume(const InterWorm::Token::Type type) {
        if (Check(type)) return Advance();
        throw std::exception("Parse Error!");
    }

    const InterWorm::Token& Builder::Current() const {
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