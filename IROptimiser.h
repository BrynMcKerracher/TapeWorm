/**
 * @file IROptimiser.h
 * @author Bryn McKerracher
 **/
#ifndef TAPEWORM_IROPTIMISER_H
#define TAPEWORM_IROPTIMISER_H

#include "Op.h"

#include <vector>

namespace TapeWorm::InterWorm {
    class IROptimiser {
    public:
        std::vector<Op::Type> Optimise(const std::vector<Op::Type>& ops);
    };
} // TapeWorm

#endif //TAPEWORM_IROPTIMISER_H
