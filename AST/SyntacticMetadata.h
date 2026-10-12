#ifndef TAPEWORM_SYNTACTICMETADATA_H
#define TAPEWORM_SYNTACTICMETADATA_H

#include "SyntacticNode.h"
#include "MetaTerminalNode.h"
#include <map>
#include <cstdint>

namespace TapeWorm::AST {
    struct SyntacticMetadata {
        MetaTerminalNode::MetaTerminalType type;
        bool isMetaTerminal = false;
        bool isReadWriteNode = false;
        int64_t volatility = 0;
        std::map<int64_t, uint8_t> addresses;

        explicit SyntacticMetadata(const SyntacticNode* node);

        static bool IsMetaTerminal(const SyntacticNode* node);
        static bool IsReadWriteNode(const SyntacticNode* node);
        static int64_t CalculateVolatility(const SyntacticNode* node);
        void PopulateAddresses(const SyntacticNode* node);
        MetaTerminalNode::MetaTerminalType DetermineType(const SyntacticNode* node);
    };
} // TapeWorm

#endif //TAPEWORM_SYNTACTICMETADATA_H
