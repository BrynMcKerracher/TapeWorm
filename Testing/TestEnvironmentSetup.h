#ifndef TAPEWORMTESTS_TESTENVIRONMENTSETUP_H
#define TAPEWORMTESTS_TESTENVIRONMENTSETUP_H

#include <sstream>

namespace TapeWorm::Test {
    class TestEnvironmentSetup {
    public:
        TestEnvironmentSetup();
        ~TestEnvironmentSetup();

        std::string GetProgramOutput() const;
    private:
        std::stringstream stream;
        std::streambuf* buffer;
    };
} // TapeWorm

#endif //TAPEWORMTESTS_TESTENVIRONMENTSETUP_H
