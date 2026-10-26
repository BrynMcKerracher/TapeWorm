#include "TestEnvironmentSetup.h"
#include "../Util.h"

#include <iostream>
#include <sstream>

namespace TapeWorm::Test {
    TestEnvironmentSetup::TestEnvironmentSetup() {
        coutBuffer = std::cout.rdbuf();
        std::cout.rdbuf(coutStream.rdbuf());

        cinBuffer = std::cin.rdbuf();
        std::cin.rdbuf(cinStream.rdbuf());
    }

    TestEnvironmentSetup::~TestEnvironmentSetup() {
        std::cout.rdbuf(coutBuffer);
        std::cin.rdbuf(cinBuffer);
    }

    void TestEnvironmentSetup::CreateInputFromFile(const std::string &inputFileName) {
        std::stringstream file;
        file << Util::FileToString(inputFileName);
        std::string temp;
        while (std::getline(file, temp)) {
            cinStream << temp << '\0';
        }
    }

    std::string TestEnvironmentSetup::GetProgramOutput() const {
        return coutStream.str();
    }
} // TapeWorm