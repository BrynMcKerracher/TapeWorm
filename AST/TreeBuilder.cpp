/**
 * @file Builder.cpp
 * @author brynm
 */
#include "TreeBuilder.h"
#include "AtomicNode.h"

#include <iostream>

namespace TapeWorm::AST {
     Global TreeBuilder::BuildAST(const std::vector<InterWorm::Token> &tokens) {
        tokenStream = tokens;
        blockNodes = {};
        current = 0;

        return BuildGlobalNode();
    }

     Global TreeBuilder::BuildGlobalNode() {
         Global globalNode = std::make_unique<GlobalNode>();
         while (current < tokenStream.size()) {
             globalNode->subNodes.emplace_back(BuildNextNode());
         }
         return globalNode;
     }

     Atomic TreeBuilder::BuildAtomicNode() {
         switch (Current().type) {
             case InterWorm::Token::InputCell:
             case InterWorm::Token::OutputCell: {
                 if (!blockNodes.empty()) blockNodes.top()->isReadWrite = true;
                 break;
             }
             default: break;
         }
         return std::make_unique<AtomicNode>(Current());
     }

     Node TreeBuilder::BuildNextNode() {
        Node node = nullptr;
        if (Match(InterWorm::Token::Type::JumpIfZero)) {
            node = BuildBlockNode();
        }
        else node = BuildAtomicNode();
        Advance();
        return node;
    }

     Node TreeBuilder::BuildBlockNode() {
        Block block = std::make_unique<BlockNode>();

        const std::size_t blockDepth = blockNodes.size();
        blockNodes.emplace(block.get());
        while (Current().type != InterWorm::Token::JumpNotZero and blockNodes.size() > blockDepth) {
            block->subNodes.push_back(BuildNextNode());
        }
        blockNodes.pop();

        return block;
    }

    bool TreeBuilder::Match(const InterWorm::Token::Type token) {
        if (Check(token)) {
            Advance();
            return true;
        }
        return false;
    }

    bool TreeBuilder::Check(const InterWorm::Token::Type type) const {
        if (AtEndOfStream()) return false;
        return Current().type == type;
    }

    bool TreeBuilder::AtEndOfStream() const {
        return Current().type == InterWorm::Token::Type::EndOfFile;
    }

    InterWorm::Token TreeBuilder::Advance() {
        if (!AtEndOfStream()) current++;
        return Previous();
    }

    const InterWorm::Token& TreeBuilder::Current() const {
        return tokenStream[current];
    }

    const InterWorm::Token& TreeBuilder::Previous() const {
        return tokenStream[current - 1];
    }

    void TreeBuilder::UpdateStatementStatus(const Block &block) {
        block->isStatement = IsStatement(block.get());
    }

    bool TreeBuilder::IsStatement(const BlockNode *blockNode) {
         int64_t pointerOffset = 0;
         for (const Node& node : blockNode->subNodes) {
             const auto [type, length] = reinterpret_cast<AtomicNode*>(node.get())->token;
             if (type == InterWorm::Token::IncPointer) pointerOffset += length;
             else if (type == InterWorm::Token::DecPointer) pointerOffset -= length;
         }
         return pointerOffset == 0;
     }
}