#include "gtest/gtest.h"

#include "../src/executecommand.hpp"
#include "../src/parser.hpp"
#include "../src/testexecute.hpp"
#include <string>
#include <vector>

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

// Unit test to test the Test class implementation
TEST(TestExecuteTest, SimpleCommand) {
    vector<string> command;
    command.push_back("test");
    command.push_back("-e");
    command.push_back("src/");
    ExecuteCommand* executable = new TestExecute(command, ";");
    executable->execute();
    EXPECT_TRUE(executable->get_success());
}
TEST(TestExecuteTest, SimpleFail) {
    vector<string> command;
    command.push_back("test");
    command.push_back("nonexistant/file/path");
    ExecuteCommand* test = new TestExecute(command, ";");
    test->execute();
    EXPECT_FALSE(test->get_success());
}
TEST(TestExecuteTest, SimpleBracket) {
    vector<string> command;
    command.push_back("[");
    command.push_back("-e");
    command.push_back("src/");
    command.push_back("]");
    ExecuteCommand* test = new TestExecute(command, ";");
    test->execute();
    EXPECT_TRUE(test->get_success());
}
TEST(TestExecuteTest, fflag) {
    vector<string> command;
    command.push_back("test");
    command.push_back("-f");
    command.push_back("src/testexecute.hpp");
    ExecuteCommand* test = new TestExecute(command, ";");
    test->execute();
    EXPECT_TRUE(test->get_success());
}
TEST(TestExecuteTest, dflag) {
    vector<string> command;
    command.push_back("test");
    command.push_back("-d");
    command.push_back("src/");
    ExecuteCommand* test = new TestExecute(command, ";");
    test->execute();
    EXPECT_TRUE(test->get_success());
}
TEST(TestExecuteTest, fflagFail) {
    vector<string> command;
    command.push_back("test");
    command.push_back("-f");
    command.push_back("~/assignment-assign_jeff_ford/prototype/");
    ExecuteCommand* test = new TestExecute(command, ";");
    test->execute();
    EXPECT_FALSE(test->get_success());
}
TEST(TestExecuteTest, dflagFail) {
    vector<string> command;
    command.push_back("test");
    command.push_back("-d");
    command.push_back("~/assignment-assign_jeff_ford/src/testexecute.hpp");
    ExecuteCommand* test = new TestExecute(command, ";");
    test->execute();
    EXPECT_FALSE(test->get_success());
}

// main() function to execute tests 
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
