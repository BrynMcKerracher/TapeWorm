/**
 * @file Analyser.cpp
 * @author Bryn McKerracher
 */
#include "Analyser.h"
#include "../AST/TerminalNode.h"
#include "../AST/SyntacticNode.h"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace TapeWorm::AST::Debug {
    void Analyser::Analyse(const Global &globalNode) {
        blockMap.clear();
        globalNode->AcceptAnyVisitor(this);
    }

    std::any Analyser::VisitTerminal(TerminalNode *node) {
        nodesVisited++;

        std::stringstream ss;
        ss << node->token.length;

        return InterWorm::Token::ToString(node->token) + ss.str();
    }

    std::any Analyser::VisitSyntactic(SyntacticNode *node) {
        nodesVisited++;
        std::string block = "[";
        for (const Node& subNode : node->subNodes) {
            block += std::any_cast<std::string>(subNode->AcceptAnyVisitor(this));
        }
        block += ']';
        blockMap[block]++;
        return std::string("#");
    }

    std::any Analyser::VisitGlobal(GlobalNode *node) {
        nodesVisited++;
        for (const Node& subNodes : node->subNodes) {
            subNodes->AcceptAnyVisitor(this);
        }

        std::vector<std::pair<std::string, uint64_t> > sorted;
        for (auto& i : blockMap) {
            sorted.emplace_back(i);
        }
        std::ranges::sort(sorted, [&](auto& x, auto& y) -> bool {
            return x.second < y.second;
        });

        std::cout << "Count:\tBlock:\n";
        for (const auto& [block, frequency] : sorted) {
            std::cout << frequency << "\t" << block << "\n";
        }
        std::cout << "ASTAnalysis visited " << nodesVisited << " nodes\n";
        return std::string("");
    }

    std::any Analyser::VisitMetaTerminal(MetaTerminalNode *node) {
        std::string str = "{";
        switch (node->metaType) {
            case MetaTerminalNode::Generic:  str += "GEN"; break;
            case MetaTerminalNode::Move:     str += "MOV"; break;
            case MetaTerminalNode::Multiply: str += "MUL"; break;
            default: break;
        }
        str += "}";
        return str;
    }
}
