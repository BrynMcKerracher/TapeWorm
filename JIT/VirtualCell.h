#ifndef TAPEWORMTESTS_VIRTUALCELL_H
#define TAPEWORMTESTS_VIRTUALCELL_H

#include <cstdint>

namespace TapeWorm::JIT {
    using Cell = uint8_t;

    struct VirtualCell {
        void Multiply(int32_t factor);
        void Add(int32_t addition);

        int32_t additionAggregate = 0;
        int32_t factorAggregate = 0;
    };
} // TapeWorm

#endif //TAPEWORMTESTS_VIRTUALCELL_H
