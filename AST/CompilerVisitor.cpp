#include "CompilerVisitor.h"

#include <filesystem>
#include <iostream>

#include "AtomicNode.h"
#include "BlockNode.h"
#include "GlobalNode.h"

#include <map>

#include "LeafStatementNode.h"

namespace TapeWorm::AST {
    std::vector<InterWorm::Op::Type> CompilerVisitor::Visit(const Global& node) {
        ops.clear();
        node->Accept(this);
        return ops;
    }

    void CompilerVisitor::VisitAtomic(AtomicNode* node) {
        const InterWorm::Token::Type tokenType = node->token.type;
        const std::size_t data = node->token.length;
        switch (tokenType) {
            case InterWorm::Token::IncPointer: {
                if (data > 1) {
                    ops.push_back(InterWorm::Op::Type::AddPointer);
                    ops.push_back(static_cast<InterWorm::Op::Type>(data));
                }
                else ops.push_back(InterWorm::Op::Type::IncPointer);
                break;
            }
            case InterWorm::Token::DecPointer: {
                if (data > 1) {
                    ops.push_back(InterWorm::Op::Type::SubPointer);
                    ops.push_back(static_cast<InterWorm::Op::Type>(data));
                }
                else ops.push_back(InterWorm::Op::Type::DecPointer);
                break;
            }
            case InterWorm::Token::IncCell: {
                if (data > 1) {
                    ops.push_back(InterWorm::Op::Type::AddImmediate);
                    ops.push_back(static_cast<InterWorm::Op::Type>(data));
                }
                else ops.push_back(InterWorm::Op::Type::IncCell);
                break;
            }
            case InterWorm::Token::DecCell: {
                if (data > 1) {
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
            case InterWorm::Token::Move: {
                ops.push_back(InterWorm::Op::Type::Add);
                ops.push_back(static_cast<InterWorm::Op::Type>(data));
                break;
            }
            case InterWorm::Token::Difference: {
                ops.push_back(InterWorm::Op::Type::Subtract);
                ops.push_back(static_cast<InterWorm::Op::Type>(data));
                break;
            }
            default: break;
        }
    }

    void CompilerVisitor::VisitBlock(BlockNode* node) {
        WriteOp(InterWorm::Op::JumpIfZero);
        for (const Node& subNode : node->subNodes) {
            subNode->Accept(this);
        }
        WriteOp(InterWorm::Op::JumpNotZero);
    }

    void CompilerVisitor::VisitGlobal(GlobalNode* node) {
        for (const Node& subNode : node->subNodes) {
            subNode->Accept(this);
        }
    }

    void CompilerVisitor::VisitLeafStatement(LeafStatementNode *node) {
        for (auto& [addr, factor] : node->addresses) {
            WriteOp(InterWorm::Op::Add);

            std::cout << "CV Addr: " << addr << "\n";
            std::cout << "CV Factor: " << (int)factor << "\n";

            WriteInt64(addr);
            WriteOp(static_cast<InterWorm::Op::Type>(factor));
        }
    }

    void CompilerVisitor::WriteInt64(const int64_t n) {
        for (uint64_t i = 0; i < sizeof(n); ++i) {
            WriteOp(static_cast<InterWorm::Op::Type>(n >> (8 * i)));
        }
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
