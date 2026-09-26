/**
 * @file SyntacticNode.h
 * @author Bryn McKerracher
 **/
#ifndef TAPEWORM_SYNTACTICNODE_H
#define TAPEWORM_SYNTACTICNODE_H

#include "NodeBase.h"

#include <vector>
#include <memory>

namespace TapeWorm::AST {
    struct SyntacticNode : NodeBase {
        std::vector<Node> subNodes;

        SyntacticNode();

        void AcceptReadOnlyVisitor(NodeReadOnlyVisitor* visitor) override;
        NodeBase* AcceptReadWriteVisitor(NodeReadWriteVisitor *visitor) override;
    };

    typedef std::unique_ptr<SyntacticNode> Syntactic;
}

#endif //TAPEWORM_BLOCKNODE_H