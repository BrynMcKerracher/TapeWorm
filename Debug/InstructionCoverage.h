#ifndef TAPEWORMTESTS_INSTRUCTIONCOVERAGE_H
#define TAPEWORMTESTS_INSTRUCTIONCOVERAGE_H

#include "../Op.h"
#include <vector>

namespace TapeWorm::AST::Debug {
    class InstructionCoverage {
    public:
        static void ListCoverage(const std::vector<InterWorm::Op::Type>& stream);
    };
} // TapeWorm

#endif //TAPEWORMTESTS_INSTRUCTIONCOVERAGE_H
