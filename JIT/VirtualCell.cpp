#include "VirtualCell.h"

namespace TapeWorm::JIT {
    void VirtualCell::Add(const int32_t addition) {
        additionAggregate += addition;
    }

    void VirtualCell::Multiply(const int32_t factor) {
        additionAggregate *= factor;
        factorAggregate *= factor;
    }
} // TapeWorm