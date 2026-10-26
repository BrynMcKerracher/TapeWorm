/**
 * @file VariantVisitor.h
 * @author Bryn McKerracher
 **/
#ifndef TAPEWORM_GENERICVISITOR_H
#define TAPEWORM_GENERICVISITOR_H

#include "NodeTypes.h"
#include <any>

namespace TapeWorm::AST {
    /**
     * @struct NodeAnyVisitor
     * @brief Visitor-pattern AST visitor with std::any return type.
     */
    struct NodeAnyVisitor {
        virtual ~NodeAnyVisitor() = default;
        virtual std::any VisitTerminal(TerminalNode* node) = 0;
        virtual std::any VisitSyntactic(SyntacticNode* node) = 0;
        virtual std::any VisitGlobal(GlobalNode* node) = 0;
        virtual std::any VisitMetaTerminal(MetaTerminalNode* node) = 0;
    };
}

#endif //TAPEWORM_GENERICVISITOR_H