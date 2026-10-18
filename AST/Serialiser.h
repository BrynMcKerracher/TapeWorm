/**
 * @file Serialiser.cpp
 * @author Bryn McKerracher
 **/
#ifndef TAPEWORM_COMPILERVISITOR_H
#define TAPEWORM_COMPILERVISITOR_H

#include "NodeReadOnlyVisitor.h"
#include "GlobalNode.h"
#include "../Op.h"

#include <vector>

namespace TapeWorm::AST {
    class Serialiser final : public NodeReadOnlyVisitor {
    public:
        std::vector<InterWorm::Op::Type> Visit(const Global& node);
    private:
        std::vector<InterWorm::Op::Type> ops;

        void VisitTerminal(TerminalNode* node) override;
        void VisitSyntactic(SyntacticNode* node) override;
        void VisitGlobal(GlobalNode* node) override;
        void VisitMetaTerminal(MetaTerminalNode *node) override;

        void WriteOps(std::initializer_list<InterWorm::Op::Type> bytes);
        void WriteOp(InterWorm::Op::Type op);
        void WriteData(std::size_t data, uint8_t numBytesToWrite = 1);
    };
} // TapeWorm

#endif //TAPEWORM_COMPILERVISITOR_H