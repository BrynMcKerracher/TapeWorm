/**
 * @file BlockNode.h
 * @author brynm
 */
#ifndef TAPEWORM_BLOCKNODE_H
#define TAPEWORM_BLOCKNODE_H

#include "NodeBase.h"
#include "NodeVisitor.h"

#include <vector>
#include <memory>

namespace TapeWorm::AST {
    struct BlockNode final : NodeBase {
        std::vector<Node> subNodes;
        bool isLeaf = true;
        bool isStatement = false;

        BlockNode();

        std::any Accept(NodeVisitor* visitor) override;
    };

    typedef std::unique_ptr<BlockNode> Block;
}

#endif //TAPEWORM_BLOCKNODE_H