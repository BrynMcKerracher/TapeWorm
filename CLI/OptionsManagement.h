/**
 * @file OptionsManagement.h
 * @author Bryn McKerracher
 **/
#ifndef TAPEWORM_OPTIONSMANAGEMENT_H
#define TAPEWORM_OPTIONSMANAGEMENT_H

#include "Environment.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace TapeWorm::CLI {
    class OptionsManagement {
    public:
        Environment ParseCommands(int argc, char** argv);
    private:
        enum ArgType {
            Data = 0,
            Flag,
            FlagWithArg
        };

        struct Argument {
            ArgType type;
            std::string name;
        };

        void ParseArgs(int argc, char** argv, Environment &env);

        std::vector<std::string> data;
        std::vector<std::string> flags;
        std::unordered_map<std::string, std::string> flagsWithArgs;

        static ArgType GetArgType(const std::string& arg);
    };
} // TapeWorm

#endif //TAPEWORM_OPTIONSMANAGEMENT_H
