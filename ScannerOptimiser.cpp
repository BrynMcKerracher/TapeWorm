#include "ScannerOptimiser.h"

namespace TapeWorm {
    ScannerOptimiser::ScannerOptimiser() {
        using InterWorm::Token;
        //Add right
        optimisations.push_back({
            Token::Type::Move,
            {Token::DecCell, Token::IncPointer, Token::IncCell, Token::DecPointer, Token::JumpNotZero},
            {{0, 2}, {1, 3}},
            1
        });
        //Add left
        optimisations.push_back({
            Token::Type::Move,
            {Token::DecCell, Token::DecPointer, Token::IncCell, Token::IncPointer, Token::JumpNotZero},
            {{0, 2}, {1, 3}},
            1,
            -1
        });
        //Sub Right
        optimisations.push_back({
            Token::Type::Difference,
            {Token::DecCell, Token::IncPointer, Token::DecCell, Token::DecPointer, Token::JumpNotZero},
            {{0, 2}, {1, 3}},
            1,
        });
        //Sub Left
        optimisations.push_back({
            Token::Type::Difference,
            {Token::DecCell, Token::DecPointer, Token::DecCell, Token::IncPointer, Token::JumpNotZero},
            {{0, 2}, {1, 3}},
            1,
            -1
        });
        //Clear (Downwards)
        optimisations.push_back({
            Token::Type::ClearCell,
            {Token::DecCell, Token::JumpNotZero},
            {},
            0,
        });
        //Clear (Overflow)
        optimisations.push_back({
            Token::Type::ClearCell,
            {Token::IncCell, Token::JumpNotZero},
            {},
            0,
        });
    }

    std::vector<InterWorm::Token> ScannerOptimiser::Optimise(const std::vector<InterWorm::Token> &tokens) const {
        std::vector<InterWorm::Token> optimisedStream;

        for (std::size_t i = 0; i < tokens.size(); ++i) {
            switch (tokens[i].type) {
                case InterWorm::Token::JumpIfZero: {
                    if (OptimisationData data = TryOptimise(tokens, i + 1); data.isAvailable) {
                        optimisedStream.push_back(data.token);
                        i += data.offset;
                    }
                    else optimisedStream.push_back(tokens[i]);
                    break;
                }
                default: {
                    optimisedStream.push_back(tokens[i]);
                    break;
                }
            }
        }
        return optimisedStream;
    }

    ScannerOptimiser::OptimisationData ScannerOptimiser::TryOptimise(const std::vector<InterWorm::Token>& tokens, std::size_t index) const {
        using InterWorm::Token;
        for (const OptimisationRequirements& optimisation : optimisations) {
            if (Match(tokens, index, optimisation.types)) {
                return {
                    true,
                    {optimisation.replacement, tokens[index + optimisation.lengthIndex].length * optimisation.factor},
                    optimisation.types.size()
                };
            }
        }
        return {};
    }

    bool ScannerOptimiser::Match(const std::vector<InterWorm::Token> &tokens, const std::size_t index, const std::vector<InterWorm::Token::Type> &types) {
        if (index + types.size() >= tokens.size()) return false;
        for (std::size_t i = index; i < types.size() + index; ++i) {
            if (tokens[i].type != types[i - index]) return false;
        }
        return true;
    }
}