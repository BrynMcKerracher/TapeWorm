/**
 * @file Util.h
 * @author brynm
 */
#ifndef TAPEWORM_UTIL_H
#define TAPEWORM_UTIL_H

#include <string>

namespace TapeWorm {
    struct Util {
        static std::string FileToString(const std::string& fileName);
    };
} // TapeWorm

#endif //TAPEWORM_UTIL_H