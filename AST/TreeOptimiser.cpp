/**
 * @file TreeOptimiser.cpp
 * @author Bryn McKerracher
 **/
#include "TreeOptimiser.h"
#include "SyntacticMetadata.h"
#include <algorithm>
#include <iostream>

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
            if (NodeBase* newNode = subNode->AcceptReadWriteVisitor(this); newNode != subNode.get()) {
                subNode.reset(newNode);
            }
        }
        //

        //Post-order analysis
        if (const SyntacticMetadata metadata(node); metadata.type != MetaTerminalNode::Generic) {
            return CreateMetaTerminal(node, metadata);
        }
        return node;
    }

    NodeBase* TreeOptimiser::VisitGlobal(GlobalNode* node) {
        for (auto& subNode : node->subNodes) {
            if (NodeBase* newNode = subNode->AcceptReadWriteVisitor(this); newNode != subNode.get()) {
                subNode.reset(newNode);
            }
        }
        return node;
    }

    NodeBase* TreeOptimiser::VisitMetaTerminal(MetaTerminalNode* node) {
        return node;
    }

    NodeBase* TreeOptimiser::CreateMetaTerminal(const SyntacticNode *node, const SyntacticMetadata& data) {
        MetaTerminalNode* metaTerminal = new MetaTerminalNode();
        metaTerminal->metaType = data.type;
        metaTerminal->addresses = data.addresses;
        return metaTerminal;
    }
} // TapeWorm