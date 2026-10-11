/**
 * @file TreeOptimiser.cpp
 * @author Bryn McKerracher
 **/
#include "TreeOptimiser.h"
#include <algorithm>

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
        //Post-order analysis
        const SyntacticMetadata metadata(node);

        //Meta-terminal opt
        if (metadata.isMetaTerminal and not metadata.isReadWriteNode and metadata.volatility == 0) {
            //return CreateMetaTerminal(node);
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

    NodeBase* TreeOptimiser::CreateMetaTerminal(const SyntacticNode *node) {
        MetaTerminalNode* metaTerminal = new MetaTerminalNode();
        int64_t relativeAddress = 0;
        for (auto& subNode : node->subNodes) {
            const TerminalNode* terminal = dynamic_cast<TerminalNode*>(subNode.get());
            switch (terminal->token.type) {
                case InterWorm::Token::IncPointer: {
                    relativeAddress += terminal->token.length;
                    break;
                }
                case InterWorm::Token::DecPointer: {
                    relativeAddress -= terminal->token.length;
                    break;
                }
                case InterWorm::Token::IncCell: {
                    metaTerminal->addresses[relativeAddress] += terminal->token.length;
                    break;
                }
                case InterWorm::Token::DecCell: {
                    metaTerminal->addresses[relativeAddress] -= terminal->token.length;
                    break;
                }
                default: break;
            }
        }
        return metaTerminal;
    }

    TreeOptimiser::SyntacticMetadata::SyntacticMetadata(const SyntacticNode *node) {
        isMetaTerminal = IsMetaTerminal(node);
        if (isMetaTerminal) {
            isReadWriteNode = IsReadWriteNode(node);
            volatility = CalculateVolatility(node);
        }
    }

    bool TreeOptimiser::SyntacticMetadata::IsMetaTerminal(const SyntacticNode *node) {
        return std::ranges::none_of(node->subNodes, [&](const auto& subNode) {
            return subNode->type != NodeBase::Terminal;
        });
    }

    bool TreeOptimiser::SyntacticMetadata::IsReadWriteNode(const SyntacticNode *node) {
        return std::ranges::any_of(node->subNodes, [&](const auto& subNode) {
            const TerminalNode* terminal = dynamic_cast<TerminalNode*>(subNode.get());
            return terminal->token.type == InterWorm::Token::OutputCell or terminal->token.type == InterWorm::Token::InputCell;
        });
    }

    int64_t TreeOptimiser::SyntacticMetadata::CalculateVolatility(const SyntacticNode *node) {
        int64_t volatility = 0;
        for (auto& subNode : node->subNodes) {
            const TerminalNode* terminal = dynamic_cast<TerminalNode*>(subNode.get());
            if (terminal->token.type == InterWorm::Token::IncPointer) {
                volatility += terminal->token.length;
            }
            else if (terminal->token.type == InterWorm::Token::DecPointer) {
                volatility -= terminal->token.length;
            }
        }
        return volatility;
    }
} // TapeWorm