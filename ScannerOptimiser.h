/**
 * @file ScannerOptimiser.h
 * @author brynm
 */
#ifndef TAPEWORM_SCANNEROPTIMISER_H
#define TAPEWORM_SCANNEROPTIMISER_H

#include "Token.h"

#include <vector>

namespace TapeWorm {
    class ScannerOptimiser {
    public:
        ScannerOptimiser();

        [[nodiscard]] std::vector<InterWorm::Token> Optimise(const std::vector<InterWorm::Token>& tokens) const;
    private:
        struct OptimisationData {
            bool isAvailable = false;
            InterWorm::Token token;
            std::size_t offset = 0;
        };

        struct OptimisationRequirements {
            InterWorm::Token::Type replacement;
            std::vector<InterWorm::Token::Type> types;
            std::vector<std::vector<std::size_t>> lengthMatches;
            std::size_t lengthIndex;
            int64_t factor = 1;
        };

        std::vector<OptimisationRequirements> optimisations;

        [[nodiscard]] OptimisationData TryOptimise(const std::vector<InterWorm::Token>& tokens, std::size_t index) const;

        static bool Match(const std::vector<InterWorm::Token>& tokens, std::size_t index, const std::vector<InterWorm::Token::Type>& types);
    };
} // TapeWorm

#endif //TAPEWORM_SCANNEROPTIMISER_H