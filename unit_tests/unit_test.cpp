#include "gtest/gtest.h"

#include "../src/parser.hpp"
#include <string>

using namespace std;

TEST(ParserTest, SimpleCommand) {
    string input = "ls -a;";
    Parser* parsed = new Parser();
    ExecuteGroup* executable = parsed->parse(input);
    string test = "ls-a";
    EXPECT_EQ(test, executable->get_command_full());
}
TEST(ParserTest, SimpleEcho) {
    string input = "echo hello";
    Parser* parsed = new Parser();
    ExecuteGroup* executable = parsed->parse(input);
    string test = "echohello";
    EXPECT_EQ(test, executable->get_command_full());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
