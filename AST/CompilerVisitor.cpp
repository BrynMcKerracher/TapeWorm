/**
 * @file CompilerVisitor.cpp
 * @author Bryn McKerracher
 **/
#include "CompilerVisitor.h"
#include "NodeTypes.h"

#include <filesystem>
#include <iostream>

namespace TapeWorm::AST {
    std::vector<InterWorm::Op::Type> CompilerVisitor::Visit(const Global& node) {
        ops.clear();
        node->AcceptReadOnlyVisitor(this);
        return ops;
    }

    void CompilerVisitor::VisitTerminal(TerminalNode* node) {
        const InterWorm::Token::Type tokenType = node->token.type;
        const std::size_t data = node->token.length;
        switch (tokenType) {
            case InterWorm::Token::IncPointer: {
                if (data > 255) {
                    ops.push_back(InterWorm::Op::Type::AddPointerExtended);
                    ops.push_back(static_cast<InterWorm::Op::Type>(data));
                    ops.push_back(static_cast<InterWorm::Op::Type>(data >> 8));
                }
                else if (data > 1) {
                    ops.push_back(InterWorm::Op::Type::AddPointer);
                    ops.push_back(static_cast<InterWorm::Op::Type>(data));
                }
                else ops.push_back(InterWorm::Op::Type::IncPointer);
                break;
            }
            case InterWorm::Token::DecPointer: {
                if (data > 255) {
                    ops.push_back(InterWorm::Op::Type::SubPointerExtended);
                    ops.push_back(static_cast<InterWorm::Op::Type>(data));
                    ops.push_back(static_cast<InterWorm::Op::Type>(data >> 8));
                }
                else if (data > 1) {
                    ops.push_back(InterWorm::Op::Type::SubPointer);
                    ops.push_back(static_cast<InterWorm::Op::Type>(data));
                }
                else ops.push_back(InterWorm::Op::Type::DecPointer);
                break;
            }
            case InterWorm::Token::IncCell: {
                if (data > 255) {
                    ops.push_back(InterWorm::Op::Type::AddImmediateExtended);
                    ops.push_back(static_cast<InterWorm::Op::Type>(data));
                    ops.push_back(static_cast<InterWorm::Op::Type>(data >> 8));
                }
                else if (data > 1) {
                    ops.push_back(InterWorm::Op::Type::AddImmediate);
                    ops.push_back(static_cast<InterWorm::Op::Type>(data));
                }
                else ops.push_back(InterWorm::Op::Type::IncCell);
                break;
            }
            case InterWorm::Token::DecCell: {
                if (data > 255) {
                    ops.push_back(InterWorm::Op::Type::SubImmediateExtended);
                    ops.push_back(static_cast<InterWorm::Op::Type>(data));
                    ops.push_back(static_cast<InterWorm::Op::Type>(data >> 8));
                }
                else if (data > 1) {
                    ops.push_back(InterWorm::Op::Type::SubImmediate);
                    ops.push_back(static_cast<InterWorm::Op::Type>(data));
                }
                else ops.push_back(InterWorm::Op::Type::DecCell);
                break;
            }
            case InterWorm::Token::ClearCell: ops.push_back(InterWorm::Op::Type::ClearCell); break;
            case InterWorm::Token::InputCell: ops.push_back(InterWorm::Op::Type::InputCell); break;
            case InterWorm::Token::OutputCell: ops.push_back(InterWorm::Op::Type::OutputCell); break;
            case InterWorm::Token::JumpIfZero: ops.push_back(InterWorm::Op::Type::JumpIfZero); break;
            case InterWorm::Token::JumpNotZero: ops.push_back(InterWorm::Op::Type::JumpNotZero); break;
            default: break;
        }
    }

    void CompilerVisitor::VisitSyntactic(SyntacticNode* node) {
        WriteOp(InterWorm::Op::JumpIfZero);
        for (const Node& subNode : node->subNodes) {
            subNode->AcceptReadOnlyVisitor(this);
        }
        WriteOp(InterWorm::Op::JumpNotZero);
    }

    void CompilerVisitor::VisitGlobal(GlobalNode* node) {
        for (const Node& subNode : node->subNodes) {
            subNode->AcceptReadOnlyVisitor(this);
        }
    }

    void CompilerVisitor::VisitMetaTerminal(MetaTerminalNode *node) {
        //Further optimisation to occur here later
    }

    void CompilerVisitor::WriteOps(const std::initializer_list<InterWorm::Op::Type> bytes) {
        for (const InterWorm::Op::Type byte : bytes) {
            ops.push_back(static_cast<InterWorm::Op::Type>(byte));
        }
    }

    void CompilerVisitor::WriteOp(const InterWorm::Op::Type op) {
        ops.push_back(op);
    }
}
