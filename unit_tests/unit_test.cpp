#include "gtest/gtest.h"

#include "../src/executecommand.hpp"
#include "../src/parser.hpp"
#include "../src/testexecute.hpp"
#include "../src/outputredirect.hpp"
#include <string>
#include <vector>

using namespace std;

char** create_charstar(const string & input) {
    char* cmd;
    vector<char*> args;
    char* temp;
    cmd = (char*)input.c_str();
    temp = strtok(cmd, " ");
    while (temp != NULL) {
	if (temp != '\0') {
	    args.push_back(temp);
	}
	temp = strtok(NULL, " ");
    }
    char** arguments = new char*[args.size() + 1];
    for (unsigned int i = 0; i < args.size(); ++i) {
	arguments[i] = args.at(i);
    }
    arguments[args.size()] = NULL;
    
    return arguments;
}

// Unit tests on Parser class, which tests user input parsing functionality
TEST(ParserTest, SimpleCommand) {
    string input = "ls -a;";
    Parser* parsed = new Parser();
    vector<string> v = parsed->parse(input);
    vector<string> t;
    t.push_back("ls -a");
    t.push_back(";");
    EXPECT_EQ(t, v);
}
TEST(ParserTest, SimpleEcho) {
    string input = "echo hello";
    Parser* parsed = new Parser();
    vector<string> v = parsed->parse(input);
    vector<string> t;
    t.push_back("echo hello");
    EXPECT_EQ(t, v);
}
TEST(ParserTest, ChainedCommand) {
    string input = "ls -a; echo hello";
    Parser* parsed = new Parser();   
    vector<string> v = parsed->parse(input);
    vector<string> t;
    t.push_back("ls -a");
    t.push_back(";");
    t.push_back("echo hello");
    EXPECT_EQ(t, v);
}
TEST(ParserTest, QuoteEcho) {
    string input = "echo \"hello world\"";  
    Parser* parsed = new Parser();   
    vector<string> v = parsed->parse(input);
    vector<string> t;
    t.push_back("echo hello world");
    EXPECT_EQ(t, v);
}
TEST(ParserTest, ConnectorEcho) {
    string input = "echo \"Hello && World\""; 
    Parser* parsed = new Parser();   
    vector<string> v = parsed->parse(input);
    vector<string> t;
    t.push_back("echo Hello && World");
    EXPECT_EQ(t, v);
}

// Unit test to ensure that arguments will execute
TEST(ExecuteTest, SimpleComand) {
    string input = "ls -a";
    Parser* parsed = new Parser();
    EXPECT_TRUE(parsed->execute(parsed->parse(input)));
}
TEST(ExecuteTest, EchoCommand) {
    string input = "echo hello world";
    Parser* parsed = new Parser();
    EXPECT_TRUE(parsed->execute(parsed->parse(input)));
}
TEST(ExecuteTest, BadCommand) {
    string input = "cd test";
    Parser* parsed = new Parser();
    EXPECT_FALSE(parsed->execute(parsed->parse(input)));
}

// Unit test to test the Test class implementation
TEST(TestExecuteTest, SimpleCommand) {
    string input = "test -e src/";
    Parser* parsed = new Parser();
    EXPECT_TRUE(parsed->execute(parsed->parse(input)));
}
TEST(TestExecuteTest, SimpleFail) {
    string input = "test nonexistant/file/path";
    Parser* parsed = new Parser();
    EXPECT_FALSE(parsed->execute(parsed->parse(input)));
}
TEST(TestExecuteTest, SimpleBracket) {
    string input = "[ -e src/ ]";
    Parser* parsed = new Parser();
    EXPECT_TRUE(parsed->execute(parsed->parse(input)));
}
TEST(TestExecuteTest, fflag) {
    string input = "test -f src/testexecute.hpp";
    Parser* parsed = new Parser();
    EXPECT_TRUE(parsed->execute(parsed->parse(input)));
}
TEST(TestExecuteTest, dflag) {
    string input = "test -d src/";
    Parser* parsed = new Parser();
    EXPECT_TRUE(parsed->execute(parsed->parse(input)));
}
TEST(TestExecuteTest, fflagFail) {
    string input = "test -f ~/assignment-assign_jeff_ford/prototype/";
    Parser* parsed = new Parser();
    EXPECT_FALSE(parsed->execute(parsed->parse(input)));
}
TEST(TestExecuteTest, dflagFail) {
    string input = "test -d ~/assignment-assign_jeff_ford/src/testexecute.hpp";
    Parser* parsed = new Parser();
    EXPECT_FALSE(parsed->execute(parsed->parse(input)));
}

// Unit test on output redirection
TEST(TestOutRedirect, EchoTest) {
    string input = "echo \"Hello World!\"";
    string output = "output.txt";
    ExecuteCommand* execute = new Execute(create_charstar(input), ";");
    OutRedirect* out = new OutRedirect(execute, output);
    EXPECT_EQ(true, out->execute());
}
TEST(TestOutRedirect, PWDOut) {
    string input = "pwd";
    string output = "outfile";
    ExecuteCommand* execute = new Execute(create_charstar(input), ";");
    OutRedirect* out = new OutRedirect(execute, output);
    EXPECT_EQ(true, out->execute());
}

// main() function to execute tests 
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
