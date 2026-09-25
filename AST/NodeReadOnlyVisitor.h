/**
 * @file NodeReadOnlyVisitor.h
 * @author Bryn McKerracher
 */
#ifndef TAPEWORM_NODEREADONLYVISITOR_H
#define TAPEWORM_NODEREADONLYVISITOR_H

#include "NodeTypes.h"

namespace TapeWorm::AST {
    /**
     * @struct NodeReadOnlyVisitor
     * @brief Visitor-pattern AST visitor with red-only capability.
     *
     * This pattern is primarily for converting the AST into another form,
     * e.g., an intermediate language.
     */
    struct NodeReadOnlyVisitor {
        virtual ~NodeReadOnlyVisitor() = default;
        virtual void VisitTerminal(TerminalNode* node) = 0;
        virtual void VisitSyntactic(SyntacticNode* node) = 0;
        virtual void VisitGlobal(GlobalNode* node) = 0;
        virtual void VisitLeafStatement(MetaTerminalNode* node) = 0;
    };
}

#endif