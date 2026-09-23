/**
 * @file Builder.cpp
 * @author Bryn McKerracher
 */
#include "Builder.h"
#include "TerminalNode.h"

namespace TapeWorm::AST {
     Global Builder::BuildAST(const std::vector<InterWorm::Token> &tokens) {
        inputTokens = tokens;
        currentTokenIndex = 0;

        Global ast = BuildGlobalNode();

        inputTokens.clear();
        currentTokenIndex = 0;

        return ast;
    }

     Global Builder::BuildGlobalNode() {
         Global globalNode = std::make_unique<GlobalNode>();
         while (currentTokenIndex < inputTokens.size()) {
             globalNode->subNodes.emplace_back(BuildNextNode());
         }
         return globalNode;
     }

     Terminal Builder::BuildTerminalNode() const {
         return std::make_unique<TerminalNode>(Current());
     }

     Syntactic Builder::BuildSyntacticNode() {
        Syntactic syntactic = std::make_unique<SyntacticNode>();

        const std::size_t localSyntacticDepth = currentSyntacticDepth++;
        while (Current().type != InterWorm::Token::JumpNotZero and currentSyntacticDepth > localSyntacticDepth) {
            syntactic->subNodes.push_back(BuildNextNode());
        }
        currentSyntacticDepth--;

        return syntactic;
    }

    Node Builder::BuildNextNode() {
         Node node = nullptr;
         if (Match(InterWorm::Token::Type::JumpIfZero)) {
             node = BuildSyntacticNode();
         }
         else node = BuildTerminalNode();
         Advance();
         return node;
    }

    bool Builder::Match(const InterWorm::Token::Type token) {
        if (Check(token)) {
            Advance();
            return true;
        }
        return false;
    }

    bool Builder::Check(const InterWorm::Token::Type type) const {
        if (AtEndOfTokens()) return false;
        return Current().type == type;
    }

    bool Builder::AtEndOfTokens() const {
        return Current().type == InterWorm::Token::Type::EndOfFile;
    }

    InterWorm::Token Builder::Advance() {
        if (!AtEndOfTokens()) currentTokenIndex++;
        return Previous();
    }

    const InterWorm::Token& Builder::Current() const {
        return inputTokens[currentTokenIndex];
    }

    const InterWorm::Token& Builder::Previous() const {
        return inputTokens[currentTokenIndex - 1];
    }
}