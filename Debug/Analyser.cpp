/**
 * @file Analyser.cpp
 * @author Bryn McKerracher
 */
#include "Analyser.h"
#include "../AST/TerminalNode.h"
#include "../AST/SyntacticNode.h"

#include <algorithm>
#include <iostream>

namespace TapeWorm::AST::Debug {
    void Analyser::Analyse(const Global &globalNode) {
        blockMap.clear();
        globalNode->AcceptReadOnlyVisitor(this);
    }

    void Analyser::VisitTerminal(TerminalNode *node) {
        nodesVisited++;
        std::cout << InterWorm::Token::ToString(node->token);
    }

    void Analyser::VisitSyntactic(SyntacticNode *node) {
        nodesVisited++;
        std::string block = "[";
        for (const Node& subNode : node->subNodes) {
            subNode->AcceptReadOnlyVisitor(this);
        }
        block += ']';
        blockMap[block]++;
        std::cout << block;
    }

    void Analyser::VisitGlobal(GlobalNode *node) {
        nodesVisited++;
        for (const Node& subNodes : node->subNodes) {
            subNodes->AcceptReadOnlyVisitor(this);
        }
        std::size_t totalBlocks = 0;
        std::vector<std::pair<std::string, std::size_t>> pairs;
        for (auto & itr : blockMap) {
            pairs.emplace_back(itr);
            totalBlocks += itr.second;
        }

        std::ranges::sort(pairs, [=](const std::pair<std::string, std::size_t>& a, const std::pair<std::string, std::size_t>& b)
        {
            return a.second > b.second;
        });

        std::cout << "Block: \tCount:\tPercentage:\n";
        for (const auto& [block, frequency] : pairs) {
            std::cout << block << "\t" << frequency << "\t" << (100.f * static_cast<float>(frequency) / static_cast<float>(totalBlocks)) << "%\n";
        }
        std::cout << "ASTAnalysis visited " << nodesVisited << " nodes\n";
    }

    void Analyser::VisitLeafStatement(MetaTerminalNode *node) {

    }
}
