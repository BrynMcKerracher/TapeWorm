#ifndef TAPEWORM_NODEVISITOR_H
#define TAPEWORM_NODEVISITOR_H

#include <memory>
#include <any>

namespace TapeWorm::AST {
    struct NodeVisitor {
        virtual ~NodeVisitor() = default;

        virtual std::any VisitAtomic(struct AtomicNode* node) = 0;
        virtual std::any VisitBlock(struct BlockNode* node) = 0;
        virtual std::any VisitGlobal(struct GlobalNode* node) = 0;
    };
}

#endif