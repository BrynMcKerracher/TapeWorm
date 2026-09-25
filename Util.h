/**
 * @file Util.h
 * @author Bryn McKerracher
 **/
#ifndef TAPEWORM_UTIL_H
#define TAPEWORM_UTIL_H

#include <string>

namespace TapeWorm {
    struct Util {
        static std::string BrainFuckFileToString(const std::string& fileName);
    };
} // TapeWorm

#endif //TAPEWORM_UTIL_H