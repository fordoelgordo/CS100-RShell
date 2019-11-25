#include "gtest/gtest.h"

#include "../src/parser.hpp"
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

//Unit tests to ensure Parenthesis are parsed correctly
TEST(ParParserTest, SimpleCommand) {
	vector<string> test = {"(echo hello world)"};
	Parser *parse = new Parser();
	parse->parse_par(test);
	string parsed = test.at(0);
	EXPECT_EQ("echo hello world", parsed);
}

TEST(ParParserTest, AssignmentCommand) {
        vector<string> test = {"(echo A && echo B) || (echo C && echo D)"};
        Parser *parse = new Parser();
        parse->parse_par(test);
        string parsed = test.at(0);
        EXPECT_EQ("echo A && echo B || echo C && echo D", parsed);
}

TEST(ParParserTest, NestedCommand) {
        vector<string> test = {"((echo hello world && echo 123) && echo test)"};
        Parser *parse = new Parser();
        parse->parse_par(test);
        string parsed = test.at(0);
        EXPECT_EQ("echo hello world && echo 123 && echo test", parsed);
}

TEST(ParParserTest, EpicCommand) {
        vector<string> test = {"(((((((e)))))ch(((o h)))))ell()o w()(orld)"};
        Parser *parse = new Parser();
        parse->parse_par(test);
        string parsed = test.at(0);
        EXPECT_EQ("echo hello world", parsed);
}

//Unit tests to ensure there is balanced Parenthesis
TEST(ParValidTest, SimpleCommand) {
	vector<string> test = {"(echo hello world && echo testing)"};
	Parser *parse = new Parser();
	bool valid = parse->check_valid_par(test);
	EXPECT_TRUE(valid);
}

TEST(ParValidTest, ChainCommand) {
        vector<string> test = {"(echo hello world && (echo testing || echo test))"};
        Parser *parse = new Parser();
        bool valid = parse->check_valid_par(test);
        EXPECT_TRUE(valid);
}

TEST(ParValidTest, FailCommand) {
        vector<string> test = {"(echo hello world && echo testing))"};
        Parser *parse = new Parser();
        bool valid = parse->check_valid_par(test);
        EXPECT_FALSE(valid);
}
 
// main() function to execute tests 
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
