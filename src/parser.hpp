#ifndef __Parser_HPP__
#define __Parser_HPP__

#include "executecommand.hpp"
#include "executegroup.hpp"
#include "execute.hpp"
#include "testexecute.hpp"
#include <string>
#include <regex>
#include <boost/tokenizer.hpp>
#include <algorithm>

using namespace std;

class Parser {
    public: 
	Parser();
	ExecuteGroup* parse(string userInput);
    private:
	char** create_charstar(const string & input);
	vector<string> create_stringvec(const string & input);
    	void print_charstar(char** input);	
	void print_vector(const vector<string> & vec); // Helper function to quickly print command vector for debugging purposes
};

#endif //__Parser_HPP__
