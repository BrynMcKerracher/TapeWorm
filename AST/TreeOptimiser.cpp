/**
 * @file TreeOptimiser.cpp
 * @author Bryn McKerracher
 **/
#include "TreeOptimiser.h"
#include "SyntacticMetadata.h"
#include "TerminalNode.h"
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


        if (node->subNodes.size() == 1 and node->subNodes[0]->type == NodeBase::Terminal) {
            const TerminalNode* terminal = dynamic_cast<const TerminalNode*>(node->subNodes[0].get());
            if (terminal->token.type == InterWorm::Token::IncPointer) {
                InterWorm::Token token = terminal->token;
                token.type = InterWorm::Token::Type::ScanForward;
                return new TerminalNode(token);
            }

            if (terminal->token.type == InterWorm::Token::DecPointer) {
                InterWorm::Token token = terminal->token;
                token.type = InterWorm::Token::Type::ScanBackward;
                return new TerminalNode(token);
            }
        }

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