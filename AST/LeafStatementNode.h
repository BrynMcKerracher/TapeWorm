/**
 * @file StatementNode.h
 * @author brynm
 */
#ifndef TAPEWORM_LEAFSTATEMENTNODE_H
#define TAPEWORM_LEAFSTATEMENTNODE_H

#include "NodeBase.h"
#include <memory>
#include <map>
#include <any>

namespace TapeWorm::AST {
    struct LeafStatementNode final : NodeBase {
        std::map<int64_t, uint8_t> addresses;

        void Accept(NodeVisitor *visitor) override;
        NodeBase* AcceptEditor(NodeEditor *editor) override;
    };
    typedef std::unique_ptr<LeafStatementNode> LeafStatement;
} // TapeWorm

#endif //TAPEWORM_LEAFSTATEMENTNODE_H