#include "InstructionCoverage.h"

#include <unordered_set>
#include <iostream>

namespace TapeWorm::AST::Debug {
    void InstructionCoverage::ListCoverage(const std::vector<InterWorm::Op::Type> &stream) {
        std::unordered_set<InterWorm::Op::Type> covered;
        for (std::size_t i = 0; i < stream.size(); ++i) {
            if (InterWorm::Op::ToString(stream[i]) != "Error") {
                covered.insert(stream[i]);
            }
            i += InterWorm::Op::NumArgs(stream[i]);
        }
        for (auto& i : covered) {
            std::cout << InterWorm::Op::ToString(i) << "\n";
        }
        std::cout << "Coverage: " << 100.f * (static_cast<float>(covered.size()) / static_cast<float>(InterWorm::Op::Count)) << "%\n";
    }
} // TapeWorm