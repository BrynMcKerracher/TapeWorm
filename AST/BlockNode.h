/**
 * @file BlockNode.h
 * @author brynm
 */
#ifndef TAPEWORM_BLOCKNODE_H
#define TAPEWORM_BLOCKNODE_H

#include "NodeBase.h"
#include <vector>
#include <memory>

namespace TapeWorm::AST {
    struct BlockNode : NodeBase {
        std::vector<Node> subNodes;
        bool isLeaf = true;
        bool isStatement = false;
        bool isReadWrite = false;

        BlockNode();

        void Accept(NodeVisitor* visitor) override;
        NodeBase* AcceptEditor(NodeEditor *editor) override;
    };

    typedef std::unique_ptr<BlockNode> Block;
}

#endif //TAPEWORM_BLOCKNODE_H