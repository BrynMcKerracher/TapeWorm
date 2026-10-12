#include "SyntacticMetadata.h"
#include "MetaTerminalNode.h"
#include "TerminalNode.h"
#include <algorithm>
#include <iostream>

namespace TapeWorm::AST {
    SyntacticMetadata::SyntacticMetadata(const SyntacticNode *node) {
        isMetaTerminal = IsMetaTerminal(node);
        type = MetaTerminalNode::Generic;
        if (isMetaTerminal) {
            isReadWriteNode = IsReadWriteNode(node);
            volatility = CalculateVolatility(node);
            PopulateAddresses(node);
            type = DetermineType(node);
        }
    }

    bool SyntacticMetadata::IsMetaTerminal(const SyntacticNode *node) {
        return std::ranges::none_of(node->subNodes, [&](const auto& subNode) {
            return subNode->type != NodeBase::Terminal;
        });
    }

    bool SyntacticMetadata::IsReadWriteNode(const SyntacticNode *node) {
        return std::ranges::any_of(node->subNodes, [&](const auto& subNode) {
            const TerminalNode* terminal = dynamic_cast<TerminalNode*>(subNode.get());
            return terminal->token.type == InterWorm::Token::OutputCell or terminal->token.type == InterWorm::Token::InputCell;
        });
    }

    int64_t SyntacticMetadata::CalculateVolatility(const SyntacticNode *node) {
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

    void SyntacticMetadata::PopulateAddresses(const SyntacticNode* node) {
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
                    addresses[relativeAddress] += terminal->token.length;
                    break;
                }
                case InterWorm::Token::DecCell: {
                    addresses[relativeAddress] -= terminal->token.length;
                    break;
                }
                default: break;
            }
        }
    }

    MetaTerminalNode::MetaTerminalType SyntacticMetadata::DetermineType(const SyntacticNode *node) {
        if (isReadWriteNode) return MetaTerminalNode::Generic;
        if (volatility == 0 and addresses[0] == 0xFF) {
            if (addresses.size() == 2 and (++addresses.begin())->second == 1) {
                return MetaTerminalNode::Move;
            }
            //return MetaTerminalNode::Multiply;
        }
        return MetaTerminalNode::Generic;
    }
} // TapeWorm