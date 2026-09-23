/**
 * @file Token.cpp
 * @author Bryn McKerracher
 */
#include "Token.h"

namespace TapeWorm::InterWorm {
    std::string Token::ToString() const {
        return ToString(*this);
    }

    std::string Token::ToString(const Token& token) {
        switch (token.type) {
            case InterWorm::Token::IncPointer:  return {">"};
            case InterWorm::Token::DecPointer:  return {"<"};
            case InterWorm::Token::IncCell:     return {"+"};
            case InterWorm::Token::DecCell:     return {"-"};
            case InterWorm::Token::ClearCell:   return {"X"};
            case InterWorm::Token::InputCell:   return {","};
            case InterWorm::Token::OutputCell:  return {"."};
            case InterWorm::Token::JumpIfZero:  return {"["};
            case InterWorm::Token::JumpNotZero: return {"]"};
            case InterWorm::Token::Move:        return {"A"};
            case InterWorm::Token::Difference:  return {"S"};
            default: return {"E"};
        }
    }
}
