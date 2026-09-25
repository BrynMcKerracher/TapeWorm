/**
 * @file Token.cpp
 * @author Bryn McKerracher
 **/
#include "Token.h"

namespace TapeWorm::InterWorm {
    std::string Token::ToString() const {
        return ToString(*this);
    }

    std::string Token::ToString(const Token& token) {
        switch (token.type) {
            case IncPointer:  return {">"};
            case DecPointer:  return {"<"};
            case IncCell:     return {"+"};
            case DecCell:     return {"-"};
            case ClearCell:   return {"X"};
            case InputCell:   return {","};
            case OutputCell:  return {"."};
            case JumpIfZero:  return {"["};
            case JumpNotZero: return {"]"};
            case Move:        return {"A"};
            case Difference:  return {"S"};
            default: return {"E"};
        }
    }
}
