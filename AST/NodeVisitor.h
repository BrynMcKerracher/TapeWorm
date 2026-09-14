#ifndef TAPEWORM_NODEVISITOR_H
#define TAPEWORM_NODEVISITOR_H

namespace TapeWorm::AST {
    struct NodeVisitor {
        virtual ~NodeVisitor() = default;
        virtual void VisitAtomic(struct AtomicNode* node) = 0;
        virtual void VisitBlock(struct BlockNode* node) = 0;
        virtual void VisitGlobal(struct GlobalNode* node) = 0;
        virtual void VisitLeafStatement(struct LeafStatementNode* node) = 0;
    };
}

#endif