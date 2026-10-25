/**
 * @file Serialiser.cpp
 * @author Bryn McKerracher
 **/
#include "Serialiser.h"
#include "NodeTypes.h"

#include <filesystem>
#include <iostream>
#include <limits>

namespace TapeWorm::AST {
    std::vector<InterWorm::Op::Type> Serialiser::Visit(const Global& node) {
        ops.clear();
        node->AcceptReadOnlyVisitor(this);
        return ops;
    }

    void Serialiser::VisitTerminal(TerminalNode* node) {
        const InterWorm::Token::Type tokenType = node->token.type;
        const std::size_t data = node->token.length;
        switch (tokenType) {
            case InterWorm::Token::IncPointer: {
                if (data > std::numeric_limits<uint8_t>::max()) {
                    WriteOp(InterWorm::Op::Type::AddPointerExtended);
                    WriteData(data, 2);
                }
                else if (data > 1) {
                    WriteOp(InterWorm::Op::Type::AddPointer);
                    WriteData(data);
                }
                else WriteOp(InterWorm::Op::Type::IncPointer);
                break;
            }
            case InterWorm::Token::DecPointer: {
                if (data > std::numeric_limits<uint8_t>::max()) {
                    WriteOp(InterWorm::Op::Type::SubPointerExtended);
                    WriteData(data, 2);
                }
                else if (data > 1) {
                    WriteOp(InterWorm::Op::Type::SubPointer);
                    WriteData(data);
                }
                else WriteOp(InterWorm::Op::Type::DecPointer);
                break;
            }
            case InterWorm::Token::IncCell: {
                if (data > std::numeric_limits<uint8_t>::max()) {
                    WriteOp(InterWorm::Op::Type::AddImmediateExtended);
                    WriteData(data, 2);
                }
                else if (data > 1) {
                    WriteOp(InterWorm::Op::Type::AddImmediate);
                    WriteData(data);
                }
                else WriteOp(InterWorm::Op::Type::IncCell);
                break;
            }
            case InterWorm::Token::DecCell: {
                if (data > std::numeric_limits<uint8_t>::max()) {
                    WriteOp(InterWorm::Op::Type::SubImmediateExtended);
                    WriteData(data, 2);
                }
                else if (data > 1) {
                    WriteOp(InterWorm::Op::Type::SubImmediate);
                    WriteData(data);
                }
                else WriteOp(InterWorm::Op::Type::DecCell);
                break;
            }
            case InterWorm::Token::ClearCell:   WriteOp(InterWorm::Op::Type::ClearCell); break;
            case InterWorm::Token::InputCell:   WriteOp(InterWorm::Op::Type::InputCell); break;
            case InterWorm::Token::OutputCell:  WriteOp(InterWorm::Op::Type::OutputCell); break;
            case InterWorm::Token::ScanForward: {
                WriteOp(InterWorm::Op::Type::Scan);
                WriteData(static_cast<int8_t>(data));
                break;
            }
            case InterWorm::Token::ScanBackward: {
                WriteOp(InterWorm::Op::Type::Scan);
                WriteData(static_cast<int8_t>(data) * -1);
                break;
            }
            default: break;
        }
    }

    void Serialiser::VisitSyntactic(SyntacticNode* node) {
        WriteOp(InterWorm::Op::JumpIfZero);
        for (const Node& subNode : node->subNodes) {
            subNode->AcceptReadOnlyVisitor(this);
        }
        WriteOp(InterWorm::Op::JumpNotZero);
    }

    void Serialiser::VisitGlobal(GlobalNode* node) {
        for (const Node& subNode : node->subNodes) {
            subNode->AcceptReadOnlyVisitor(this);
        }
    }

    void Serialiser::VisitMetaTerminal(MetaTerminalNode* node) {
        if (node->addresses.empty()) return;

        switch (node->metaType) {
            case MetaTerminalNode::Multiply: {
                node->addresses.erase(0);
                WriteOp(InterWorm::Op::Multiply);
                WriteOp(static_cast<InterWorm::Op::Type>(node->addresses.size()));
                for (const auto& [address, factor] : node->addresses) {
                    WriteOp(static_cast<InterWorm::Op::Type>(address));
                    WriteOp(static_cast<InterWorm::Op::Type>(factor));
                }
                break;
            }
            default: break;
        }
    }

    void Serialiser::WriteOps(const std::initializer_list<InterWorm::Op::Type> bytes) {
        for (const InterWorm::Op::Type byte : bytes) {
            ops.push_back(byte);
        }
    }

    void Serialiser::WriteOp(const InterWorm::Op::Type op) {
        ops.push_back(op);
    }

    void Serialiser::WriteData(const std::size_t data, const uint8_t numBytesToWrite) {
        for (uint8_t i = 0; i < numBytesToWrite; i++) {
            WriteOp(static_cast<InterWorm::Op::Type>(data >> (i * 8)));
        }
    }
}
