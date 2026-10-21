/**
 * @file Analyser.h
 * @author Bryn McKerracher
 */
#ifndef TAPEWORM_ASTANALYSER_H
#define TAPEWORM_ASTANALYSER_H

#include "../AST/GlobalNode.h"
#include "../AST/TerminalNode.h"
#include "../AST/SyntacticNode.h"
#include "../AST/MetaTerminalNode.h"
#include "../AST/NodeAnyVisitor.h"

#include <map>
#include <string>
#include <any>

namespace TapeWorm::AST::Debug {
    class Analyser : public NodeAnyVisitor {
    public:
        void Analyse(const Global& globalNode);
    private:
        std::map<std::string, std::size_t> blockMap;
        std::size_t nodesVisited = 0;

        std::any VisitTerminal(TerminalNode* node) override;
        std::any VisitSyntactic(SyntacticNode* node) override;
        std::any VisitGlobal(GlobalNode* node) override;
        std::any VisitMetaTerminal(MetaTerminalNode* node) override;
    };
}

#endif //TAPEWORM_ASTANALYSER_H