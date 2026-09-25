/**
 * @file TreeOptimiser.h
 * @author Bryn McKerracher
 */
#ifndef TAPEWORMTESTS_TREEOPTIMISER_H
#define TAPEWORMTESTS_TREEOPTIMISER_H

#include "GlobalNode.h"
#include "NodeReadWriteVisitor.h"
#include "NodeReadOnlyVisitor.h"

namespace TapeWorm::AST {
    /**
     * @class TreeOptimiser
     * @brief Traverses and optimises the AST.
     *
     * Optimisation occurs by culling/replacing existent nodes on the AST to create
     * equivalent behaviour with a smaller tree.
     **/
    class TreeOptimiser final : NodeReadWriteVisitor {
    public:
        /**
         * @brief Optimises the AST for execution speed.
         * @param node The root node of the AST.
         * @return An optimised AST.
         */
        Global Optimise(Global& node);
    private:
        NodeBase* VisitTerminal(TerminalNode* node) override;
        NodeBase* VisitSyntactic(SyntacticNode* node) override;
        NodeBase* VisitGlobal(GlobalNode* node) override;
        NodeBase* VisitMetaTerminal(MetaTerminalNode* node) override;
    };
} // TapeWorm

#endif //TAPEWORMTESTS_TREEOPTIMISER_H