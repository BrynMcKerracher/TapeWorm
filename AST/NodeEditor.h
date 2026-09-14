/**
 * @file NodeEditor.h
 * @author brynm
 */
#ifndef TAPEWORMTESTS_NODEEDITOR_H
#define TAPEWORMTESTS_NODEEDITOR_H

#include "AtomicNode.h"
#include "BlockNode.h"
#include "GlobalNode.h"
#include "LeafStatementNode.h"
#include "NodeBase.h"

namespace TapeWorm::AST {
    struct NodeEditor {
        virtual ~NodeEditor() = default;

        virtual NodeBase* VisitAtomic(AtomicNode* atomic) = 0;
        virtual NodeBase* VisitBlock(BlockNode* block) = 0;
        virtual NodeBase* VisitGlobal(GlobalNode* global) = 0;
        virtual NodeBase* VisitLeafStatement(LeafStatementNode* leafStatement) = 0;
    };
} // TapeWorm

#endif //TAPEWORMTESTS_NODEEDITOR_H