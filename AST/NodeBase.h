/**
 * @file NodeBase.h
 * @author Bryn McKerracher
 */
#ifndef TAPEWORM_NODEBASE_H
#define TAPEWORM_NODEBASE_H

#include <memory>

namespace TapeWorm::AST {
    /**
     * @struct NodeBase
     * @brief Base class for all nodes in the AST.
     *
     * To add a new Node type to the AST structure:
     *   1. Add a new Type for the class to the NodeBase::Type enum.
     *   2. Create a new subclass of NodeBase.
     *   3. Make sure the new class sets its nodeType to the new Type.
     **/
    struct NodeBase {
        /**
         * @brief Exhaustive list of all subclasses.
         *
         * These are tags for each subclass type.
         */
        enum Type {
            None,        /// No subclass should have this type.
            Terminal,    /// In the formal grammatical sense.
            Syntactic,   /// In the formal grammatical sense.
            Global,      /// AST root node type.
            MetaTerminal /// Syntactic node containing ONLY terminal statements.
        };

        /**
         * @brief Tag specifying subclass type.
         **/
        Type type = None;

        virtual ~NodeBase() = default;

        /**
         * @brief Visitor pattern entry method for read-only traversing the AST.
         * @param visitor The visitor object read-only traversing the AST.
         */
        virtual void AcceptReadOnlyVisitor(struct NodeReadOnlyVisitor* visitor) = 0;

        /**
         * @brief Visitor pattern entry method for read-write traversing the AST.
         * @param editor The visitor object read-write traversing the AST.
         * @return An updated node to replace this node.
         */
        virtual NodeBase* AcceptReadWriteVisitor(struct NodeReadWriteVisitor* editor) = 0;
    };

    typedef std::unique_ptr<NodeBase> Node;
}

#endif