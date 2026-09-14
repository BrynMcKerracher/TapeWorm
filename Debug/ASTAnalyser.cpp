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

    void ASTAnalyser::VisitAtomic(AtomicNode *node) {
        nodesVisited++;
        std::cout << InterWorm::Token::ToString(node->token);
    }

    void ASTAnalyser::VisitBlock(BlockNode *node) {
        nodesVisited++;
        std::string block = "[";
        for (const Node& subNode : node->subNodes) {
            subNode->Accept(this);
        }
        block += "]";
        blockMap[block]++;
        std::cout << block;
    }

    void ASTAnalyser::VisitGlobal(GlobalNode *node) {
        nodesVisited++;
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
        std::cout << "ASTAnalysis visited " << nodesVisited << " nodes\n";
    }

    void ASTAnalyser::VisitLeafStatement(LeafStatementNode *node) {

    }
}
