/**
 * @file TerminalNode.h
 * @author Bryn McKerracher
 */
#ifndef TAPEWORM_TERMINALNODE_H
#define TAPEWORM_TERMINALNODE_H

#include "NodeBase.h"
#include "../Token.h"

#include <memory>

namespace TapeWorm::AST {
    /**
     * @struct TerminalNode
     * @brief Represents a formal terminal InterWorm Token node in the AST.
     **/
    struct TerminalNode final : NodeBase {
        InterWorm::Token token;

        TerminalNode();
        explicit TerminalNode(const InterWorm::Token& token);

        void AcceptReadOnlyVisitor(NodeReadOnlyVisitor *visitor) override;
        NodeBase* AcceptReadWriteVisitor(NodeReadWriteVisitor *editor) override;
    };
    typedef std::unique_ptr<TerminalNode> Terminal;
}

#endif //TAPEWORM_ATOMICNODE_H