#include "CompilerVisitor.h"

#include <filesystem>
#include <iostream>

#include "AtomicNode.h"
#include "BlockNode.h"
#include "GlobalNode.h"

#include <map>

#include "asmjit/support/arena.h"

namespace TapeWorm::AST {
    std::vector<InterWorm::Op::Type> CompilerVisitor::Visit(const Global& node) {
        ops.clear();
        node->Accept(this);
        return ops;
    }

    std::any CompilerVisitor::VisitAtomic(AtomicNode* node) {
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
            default: return 0;
        }
        return 0;
    }

    std::any CompilerVisitor::VisitBlock(BlockNode* node) {
        if (node->isStatement and node->isLeaf) {
            CompileStatement(node);
            return 0;
        }

        WriteOp(InterWorm::Op::JumpIfZero);
        for (const Node& subNode : node->subNodes) {
            subNode->Accept(this);
        }
        WriteOp(InterWorm::Op::JumpNotZero);
        return 0;
    }

    std::any CompilerVisitor::VisitGlobal(GlobalNode* node) {
        for (const Node& subNode : node->subNodes) {
            subNode->Accept(this);
        }
        return 0;
    }

    void CompilerVisitor::WriteOps(const std::initializer_list<InterWorm::Op::Type> bytes) {
        for (const InterWorm::Op::Type byte : bytes) {
            ops.push_back(static_cast<InterWorm::Op::Type>(byte));
        }
    }

    void CompilerVisitor::WriteOp(const InterWorm::Op::Type op) {
        ops.push_back(op);
    }

    void CompilerVisitor::CompileStatement(const BlockNode* block) {
        WriteOp(InterWorm::Op::LoadPointer);
        std::map<int64_t, int64_t> addressFactors;
        int64_t address = 0;
        int64_t value = 0;
        std::string nodeStr;
        for (const auto & subNode : block->subNodes) {
            const AtomicNode* atomic = reinterpret_cast<AtomicNode*>(subNode.get());
            switch (atomic->token.type) {
                case InterWorm::Token::IncPointer: {
                    addressFactors[address] += value;
                    address += atomic->token.length;
                    value = 0;
                    nodeStr += ">";
                    break;
                }
                case InterWorm::Token::DecPointer: {
                    addressFactors[address] += value;
                    address -= atomic->token.length;;
                    value = 0;
                    nodeStr += "<";
                    break;
                }
                case InterWorm::Token::IncCell: {
                    value += atomic->token.length;
                    nodeStr += "+";
                    break;
                }
                case InterWorm::Token::DecCell: {
                    value -= atomic->token.length;
                    nodeStr += "-";
                    break;
                }
                default: break;
            }
        }
        addressFactors.erase(0);
        std::cout << nodeStr << "\n";
        for (auto [offset, factor] : addressFactors) {
            WriteOp(InterWorm::Op::Multiply);
            WriteOps({static_cast<InterWorm::Op::Type>(offset),
                           static_cast<InterWorm::Op::Type>(factor)});
            std::cout << "MUL " << offset << ", " << factor << "\n";
        }
        std::cout << "\n";
        WriteOp(InterWorm::Op::ClearCell);
    }
}
