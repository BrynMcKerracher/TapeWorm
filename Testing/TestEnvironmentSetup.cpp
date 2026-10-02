#include "TestEnvironmentSetup.h"

#include <iostream>

namespace TapeWorm::Test {
    TestEnvironmentSetup::TestEnvironmentSetup() {
        buffer = std::cout.rdbuf();
        std::cout.rdbuf(stream.rdbuf());
    }

    TestEnvironmentSetup::~TestEnvironmentSetup() {
        std::cout.rdbuf(buffer);
    }

    std::string TestEnvironmentSetup::GetProgramOutput() const {
        return stream.str();
    }
} // TapeWorm