/**
 * @file Environment.h
 * @author Bryn McKerracher
 **/
#ifndef TAPEWORM_ENVIRONMENT_H
#define TAPEWORM_ENVIRONMENT_H

#include <string>

namespace TapeWorm {
    struct Environment {
        const std::string Version = "v1.1.0";
        std::string SourceFileLocation;
        bool PerformASTOptimsationPass = true;
        bool InvalidEnvironment = false;
    };
} // TapeWorm

#endif //TAPEWORM_ENVIRONMENT_H
