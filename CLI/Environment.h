/**
 * @file Environment.h
 * @author Bryn McKerracher
 **/
#ifndef TAPEWORM_ENVIRONMENT_H
#define TAPEWORM_ENVIRONMENT_H

#include <string>

namespace TapeWorm {
    struct Environment {
        const std::string Version = "v1.2.0";
        std::string SourceFileLocation;
        bool PerformASTOptimisationPass = true;
        bool PerformIROptimisationPass = true;
        bool InvalidEnvironment = false;

        ~Environment();
    };
} // TapeWorm

#endif //TAPEWORM_ENVIRONMENT_H
