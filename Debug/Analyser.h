/**
 * @file Analyser.h
 * @author Bryn McKerracher
 */
#ifndef TAPEWORM_ASTANALYSER_H
#define TAPEWORM_ASTANALYSER_H

#include "../AST/NodeReadOnlyVisitor.h"
#include "../AST/GlobalNode.h"

#include <map>
#include <string>

namespace TapeWorm::AST::Debug {
    class Analyser final : public NodeReadOnlyVisitor {
    public:
        void Analyse(const Global& globalNode);
    private:
        std::map<std::string, std::size_t> blockMap;
        std::size_t nodesVisited = 0;

        void VisitTerminal(TerminalNode* node) override;
        void VisitSyntactic(SyntacticNode* node) override;
        void VisitGlobal(GlobalNode* node) override;
        void VisitLeafStatement(MetaTerminalNode *node) override;
    };
}

#endif //TAPEWORM_ASTANALYSER_H