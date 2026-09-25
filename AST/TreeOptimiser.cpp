/**
 * @file TreeOptimiser.cpp
 * @author Bryn McKerracher
 */
#include "TreeOptimiser.h"

namespace TapeWorm::AST {
    Global TreeOptimiser::Optimise(Global& node) {
        node->AcceptReadWriteVisitor(this);
        return std::move(node);
    }

    NodeBase* TreeOptimiser::VisitTerminal(TerminalNode* node) {
        return node;
    }

    NodeBase* TreeOptimiser::VisitSyntactic(SyntacticNode* node) {
        for (auto& subNode : node->subNodes) {
            NodeBase* newNode = subNode->AcceptReadWriteVisitor(this);
            if (newNode != subNode.get()) {
                subNode.reset(newNode);
            }
        }
        return node;
    }

    NodeBase* TreeOptimiser::VisitGlobal(GlobalNode* node) {
        for (auto& subNode : node->subNodes) {
            NodeBase* newNode = subNode->AcceptReadWriteVisitor(this);
            if (newNode != subNode.get()) {
                subNode.reset(newNode);
            }
        }
        return node;
    }

    NodeBase* TreeOptimiser::VisitMetaTerminal(MetaTerminalNode* node) {
        return node;
    }
} // TapeWorm