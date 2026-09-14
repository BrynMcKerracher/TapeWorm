/**
 * @file ASTAnalyser.h
 * @author brynm
 */
#ifndef TAPEWORM_ASTANALYSER_H
#define TAPEWORM_ASTANALYSER_H

#include <map>
#include <string>
#include "../AST/NodeVisitor.h"
#include "../AST/GlobalNode.h"

namespace TapeWorm::AST::Debug {
    class ASTAnalyser final : public NodeVisitor {
    public:
        void Analyse(const Global& globalNode);
    private:
        std::map<std::string, std::size_t> blockMap;
        std::size_t nodesVisited = 0;

        void VisitAtomic(AtomicNode* node) override;
        void VisitBlock(BlockNode* node) override;
        void VisitGlobal(GlobalNode* node) override;
        void VisitLeafStatement(LeafStatementNode *node) override;
    };
}

#endif //TAPEWORM_ASTANALYSER_H