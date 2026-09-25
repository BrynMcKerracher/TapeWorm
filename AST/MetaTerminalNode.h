/**
 * @file MetaTerminalNode.h
 * @author Bryn McKerracher
 **/
#ifndef TAPEWORM_METATERMINALNODE_H
#define TAPEWORM_METATERMINALNODE_H

#include "NodeBase.h"
#include <cstdint>
#include <memory>
#include <map>

namespace TapeWorm::AST {
    struct MetaTerminalNode final : NodeBase {
        std::map<int64_t, uint8_t> addresses;

        void AcceptReadOnlyVisitor(NodeReadOnlyVisitor *visitor) override;
        NodeBase* AcceptReadWriteVisitor(NodeReadWriteVisitor *editor) override;
    };
    typedef std::unique_ptr<MetaTerminalNode> LeafStatement;
} // TapeWorm

#endif //TAPEWORM_METATERMINALNODE_H