/**
 * @file Op.cpp
 * @author Bryn McKerracher
 */
#include "Op.h"

namespace TapeWorm::InterWorm {
    std::string Op::ToString(const Type type) {
        switch (type) {
            case Nop:          return "Nop";
            case IncPointer:   return "IncPointer";
            case DecPointer:   return "DecPointer";
            case IncCell:      return "IncCell";
            case DecCell:      return "DecCell";
            case ClearCell:    return "ClearCell";
            case InputCell:    return "InputCell";
            case OutputCell:   return "OutputCell";
            case JumpIfZero:   return "JumpIfZero";
            case JumpNotZero:  return "JumpNotZero";
            case AddImmediate: return "AddImmediate";
            case AddImmediateExtended: return "AddImmediateExtended";
            case SubImmediate: return "SubImmediate";
            case SubImmediateExtended: return "SubImmediateExtended";
            case AddPointer:   return "AddPointer";
            case AddPointerExtended:   return "AddPointerExtended";
            case SubPointer:   return "SubPointer";
            case SubPointerExtended:   return "SubPointerExtended";
            case Count:        return "Count";
                break;
        }
        return "Error";
    }

    std::size_t Op::NumArgs(const Type type) {
        switch (type) {
            case AddPointer:
            case SubPointer:
            case AddImmediate:
            case SubImmediate:
                return 1;
            case AddPointerExtended:
            case SubPointerExtended:
            case AddImmediateExtended:
            case SubImmediateExtended:
                return 2;
            default: return 0;
        }
    }
} // TapeWorm