/**
 * @file OptionsManagement.cpp
 * @author Bryn McKerracher
 **/
#include "OptionsManagement.h"

#include <algorithm>
#include <iostream>

namespace TapeWorm::CLI {
    Environment OptionsManagement::ParseCommands(const int argc, char** argv) {
        Environment environment;
        ParseArgs(argc, argv, environment);

        if (data.empty()) {
            environment.InvalidEnvironment = true;
            return environment;
        }

        environment.SourceFileLocation = data[0];
        const bool performOptimisation = std::ranges::find(flags, "--no-opt") == flags.end();
        environment.PerformASTOptimisationPass = std::ranges::find(flags, "--no-ast-opt") == flags.end() and performOptimisation;
        environment.PerformIROptimisationPass = std::ranges::find(flags, "--no-ir-opt") == flags.end() and performOptimisation;

        return environment;
    }

    void OptionsManagement::ParseArgs(const int argc, char **argv, Environment &env) {
        std::vector<Argument> rawArgs;
        for (std::size_t i = 1; i < argc; ++i) {
            rawArgs.push_back({
                .type = GetArgType(argv[i]),
                .name = argv[i]
            });
        }
        for (std::size_t i = 0; i < rawArgs.size(); ++i) {
            const auto&[type, name] = rawArgs[i];
            switch (type) {
                case Data: {
                    data.push_back(name);
                    break;
                }
                case Flag: {
                    flags.push_back(name);
                    break;
                }
                case FlagWithArg: {
                    if (i + 1 >= rawArgs.size() or rawArgs[i + 1].type != Data) {
                        std::cerr << "Error: No argument specified for flag '" << name << "'\n";
                        env.InvalidEnvironment = true;
                        return;
                    }
                    flagsWithArgs[name] = rawArgs[i + 1].name;
                    i++;
                    break;
                }
            }
        }
    }

    OptionsManagement::ArgType OptionsManagement::GetArgType(const std::string& arg) {
        if (arg.size() > 2 and arg[0] == '-' and arg[1] == '-') {
            return Flag;
        }
        if (arg.size() > 1 and arg[0] == '-') {
            return FlagWithArg;
        }
        return Data;
    }
} // TapeWorm