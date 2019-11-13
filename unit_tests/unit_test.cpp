#include "gtest/gtest.h"

#include "../src/parser.hpp"
#include <string>

using namespace std;

// Unit tests on Parser class, which tests user input parsing functionality
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
TEST(ParserTest, ChainedCommand) {
    string input = "ls -a; echo hello";
    Parser* parsed = new Parser();   
    ExecuteGroup* executable = parsed->parse(input);
    string test = "ls-aechohello";
    EXPECT_EQ(test, executable->get_command_full());
}
TEST(ParserTest, QuoteEcho) {
    string input = "echo \"hello world\"";  
    Parser* parsed = new Parser();   
    ExecuteGroup* executable = parsed->parse(input);
    string test = "echohelloworld";
    EXPECT_EQ(test, executable->get_command_full());
}
TEST(ParserTest, ConnectorEcho) {
    string input = "echo \"Hello && World\""; 
    Parser* parsed = new Parser();   
    ExecuteGroup* executable = parsed->parse(input);
    string test = "echoHello&&World";
    EXPECT_EQ(test, executable->get_command_full());
}

// Unit test to ensure that arguments will execute
TEST(ExecuteTest, SimpleComand) {
    char* input[] = {"ls", "-a", NULL};
    Execute* execute = new Execute(input, ";");
    execute->execute();
    EXPECT_TRUE(execute->get_success());
}
TEST(ExecuteTest, EchoCommand) {
    char* input[] = {"echo", "hello", "world", NULL};
    Execute* execute = new Execute(input, ";");
    execute->execute();
    EXPECT_TRUE(execute->get_success());
}
TEST(ExecuteTest, BadCommand) {
    char* input[] = {"cd", "test", NULL};
    Execute* execute = new Execute(input, ";");
    execute->execute();
    EXPECT_FALSE(execute->get_success());
}
   
// main() function to execute tests 
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
