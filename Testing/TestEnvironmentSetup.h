#ifndef TAPEWORMTESTS_TESTENVIRONMENTSETUP_H
#define TAPEWORMTESTS_TESTENVIRONMENTSETUP_H

#include <cstdint>
#include <sstream>

namespace TapeWorm::Test {
    class TestEnvironmentSetup {
    public:
        TestEnvironmentSetup();
        ~TestEnvironmentSetup();

        void CreateInputFromFile(const std::string& inputFileName);
        std::string GetProgramOutput() const;
    private:
        std::stringstream coutStream;
        std::stringstream cinStream;
        std::streambuf* coutBuffer;
        std::streambuf* cinBuffer;
    };
} // TapeWorm

#endif //TAPEWORMTESTS_TESTENVIRONMENTSETUP_H
