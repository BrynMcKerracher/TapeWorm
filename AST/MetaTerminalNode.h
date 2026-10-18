/**
 * @file MetaTerminalNode.h
 * @author Bryn McKerracher
 **/
#ifndef TAPEWORM_METATERMINALNODE_H
#define TAPEWORM_METATERMINALNODE_H

#include "NodeBase.h"

#include <memory>
#include <cstdint>
#include <map>

namespace TapeWorm::AST {
    struct MetaTerminalNode final : NodeBase {
        enum MetaTerminalType : uint8_t {
            Generic = 0,
            Move,
            Multiply,
            Count
        };

        MetaTerminalNode();

        std::map<int64_t, uint8_t> addresses;
        MetaTerminalType metaType = Generic;

        void AcceptReadOnlyVisitor(NodeReadOnlyVisitor *visitor) override;
        NodeBase* AcceptReadWriteVisitor(NodeReadWriteVisitor *editor) override;
    };
    typedef std::unique_ptr<MetaTerminalNode> MetaTerminal;
} // TapeWorm

#endif //TAPEWORM_METATERMINALNODE_H