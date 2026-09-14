/**
 * @file TreeOptimiser.cpp
 * @author Bryn McKerracher
 */
#include "TreeOptimiser.h"

#include <iostream>

#include "LeafStatementNode.h"

namespace TapeWorm::AST {
    Global TreeOptimiser::Optimise(Global& node) {
        node->AcceptEditor(this);
        return std::move(node);
    }

    NodeBase* TreeOptimiser::VisitAtomic(AtomicNode* node) {
        return node;
    }

    NodeBase* TreeOptimiser::VisitBlock(BlockNode* node) {
        for (auto& subNode : node->subNodes) {
            NodeBase* newNode = subNode->AcceptEditor(this);
            if (newNode != subNode.get()) {
                subNode.reset(newNode);
            }
        }
        return node;
    }

    NodeBase* TreeOptimiser::VisitGlobal(GlobalNode* node) {
        for (auto& subNode : node->subNodes) {
            NodeBase* newNode = subNode->AcceptEditor(this);
            if (newNode != subNode.get()) {
                subNode.reset(newNode);
            }
        }
        return node;
    }

    NodeBase* TreeOptimiser::VisitLeafStatement(LeafStatementNode* node) {
        return node;
    }
} // TapeWorm