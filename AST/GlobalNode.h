/**
 * @file GlobalNode.h
 * @author brynm
 */
#ifndef TAPEWORM_GLOBALNODE_H
#define TAPEWORM_GLOBALNODE_H

#include "NodeBase.h"

#include <memory>
#include <vector>

namespace TapeWorm::AST {
    struct GlobalNode final : NodeBase {
        std::vector<Node> subNodes;

        GlobalNode();

        void AcceptReadOnlyVisitor(NodeReadOnlyVisitor* visitor) override;
        NodeBase* AcceptReadWriteVisitor(NodeReadWriteVisitor* editor) override;
    };

    typedef std::unique_ptr<GlobalNode> Global;
} // TapeWorm

#endif //TAPEWORM_GLOBALNODE_H