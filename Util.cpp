/**
 * @file Util.cpp
 * @author brynm
 */
#include "Util.h"

#include <sstream>
#include <fstream>

namespace TapeWorm {
    std::string Util::FileToString(const std::string &fileName) {
        std::ifstream sourceFile(fileName);

        if (!sourceFile.is_open()) {
            throw std::exception {
                ("Unable to open source file '" + fileName + "'\n").c_str()
            };
        }

        std::stringstream fileBuffer;
        fileBuffer << sourceFile.rdbuf();
        return fileBuffer.str();
    }
} // TapeWorm