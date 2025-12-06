/**
 * @file GlobalNode.h
 * @author brynm
 */
#ifndef TAPEWORM_GLOBALNODE_H
#define TAPEWORM_GLOBALNODE_H

#include "NodeBase.h"
#include "NodeVisitor.h"

#include <memory>
#include <vector>

namespace TapeWorm::AST {
    struct GlobalNode final : NodeBase {
        std::vector<Node> subNodes;

        GlobalNode();

        std::any Accept(NodeVisitor* visitor) override;
    };

    typedef std::unique_ptr<GlobalNode> Global;
} // TapeWorm

#endif //TAPEWORM_GLOBALNODE_H