/**
 * @file AtomicNode.h
 * @author brynm
 */
#ifndef TAPEWORM_ATOMICNODE_H
#define TAPEWORM_ATOMICNODE_H

#include "NodeBase.h"
#include "../Token.h"

#include <memory>

namespace TapeWorm::AST {
    struct AtomicNode final : NodeBase {
        InterWorm::Token token;

        AtomicNode();
        explicit AtomicNode(const InterWorm::Token& token);

        void Accept(NodeVisitor *visitor) override;
        NodeBase* AcceptEditor(NodeEditor *editor) override;
    };
    typedef std::unique_ptr<AtomicNode> Atomic;
}

#endif //TAPEWORM_ATOMICNODE_H