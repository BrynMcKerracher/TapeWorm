/**
 * @file ASTAnalyser.cpp
 * @author brynm
 */
#include "ASTAnalyser.h"

#include "../AST/AtomicNode.h"
#include "../AST/BlockNode.h"

#include <algorithm>
#include <iostream>

namespace TapeWorm::AST::Debug {
    void ASTAnalyser::Analyse(const Global &globalNode) {
        blockMap.clear();
        globalNode->Accept(this);
    }

    std::any ASTAnalyser::VisitAtomic(AtomicNode *node) {
        switch (node->token.type) {
            case InterWorm::Token::IncPointer:  return std::string(">");
            case InterWorm::Token::DecPointer:  return std::string("<");
            case InterWorm::Token::IncCell:     return std::string("+");
            case InterWorm::Token::DecCell:     return std::string("-");
            case InterWorm::Token::ClearCell:   return std::string("X");
            case InterWorm::Token::InputCell:   return std::string(",");
            case InterWorm::Token::OutputCell:  return std::string(".");
            case InterWorm::Token::JumpIfZero:  return std::string("[");
            case InterWorm::Token::JumpNotZero: return std::string("]");
            case InterWorm::Token::Move:        return std::string("A");
            case InterWorm::Token::Difference:  return std::string("S");
            default: break;
        }
        return std::string("E");
    }

    std::any ASTAnalyser::VisitBlock(BlockNode *node) {
        std::string block = "[";
        for (const Node& subNode : node->subNodes) {
            block += std::any_cast<std::string>(subNode->Accept(this));
        }
        block += "]";
        blockMap[block]++;
        return block;
    }

    std::any ASTAnalyser::VisitGlobal(GlobalNode *node) {
        for (const Node& subNodes : node->subNodes) {
            subNodes->Accept(this);
        }
        std::size_t totalBlocks = 0;
        std::vector<std::pair<std::string, std::size_t>> pairs;
        for (auto itr = blockMap.begin(); itr != blockMap.end(); ++itr) {
            pairs.emplace_back(*itr);
            totalBlocks += itr->second;
        }

        std::ranges::sort(pairs, [=](const std::pair<std::string, std::size_t>& a, const std::pair<std::string, std::size_t>& b)
        {
            return a.second > b.second;
        });

        std::cout << "Block: \tCount:\tPercentage:\n";
        for (const auto& [block, frequency] : pairs) {
            std::cout << block << "\t" << frequency << "\t" << (100.f * (float)frequency / (float)totalBlocks) << "%\n";
        }
        return 0;
    }
}
