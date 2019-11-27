#ifndef __Parser_HPP__
#define __Parser_HPP__

#include "executecommand.hpp"
#include "exit.hpp"
//#include "executegroup.hpp"
#include "execute.hpp"
#include "testexecute.hpp"
#include <string>
#include <regex>
#include <boost/tokenizer.hpp>
#include <algorithm>
#include <stack>
#include <queue>
#include "and.hpp"
#include "or.hpp"
#include "semi.hpp"

using namespace std;

class Parser {
    public: 
	Parser();
	vector<string> parse(string userInput);
	bool execute(vector<string> userInput);
    protected:
	char** create_charstar(const string & input);
	vector<string> create_stringvec(const string & input);
    	void print_charstar(char** input);	
	void print_vector(const vector<string> & vec); // Helper function to quickly print command vector for debugging purposes
	bool check_paren(const vector<string> & input);
	vector<string> eval_precedence(vector<string> & input);
};

#endif //__Parser_HPP__
