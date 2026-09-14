/**
 * @file Node.h
 * @author brynm
 */
#ifndef TAPEWORM_EXPRESSION_H
#define TAPEWORM_EXPRESSION_H

#include <memory>
#include <any>

namespace TapeWorm::AST {
    struct NodeBase {
        enum Type {
            None,
            Atomic,
            Block,
            Global,
            LeafStatement
        };

        Type nodeType = None;

        virtual ~NodeBase() = default;
        virtual void Accept(struct NodeVisitor* visitor) = 0;
        virtual NodeBase* AcceptEditor(struct NodeEditor* editor) = 0;
    };

    typedef std::unique_ptr<NodeBase> Node;
}

#endif