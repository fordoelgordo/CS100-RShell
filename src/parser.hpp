#ifndef __Parser_HPP__
#define __Parser_HPP__

#include "executecommand.hpp"
#include "executegroup.hpp"
#include "execute.hpp"
#include <string>
#include <regex>
#include <boost/tokenizer.hpp>
#include <algorithm>
#include <string.h>

using namespace std;

class Parser {
    public: 
	Parser();
	ExecuteGroup* parse(string userInput);

	void parse_par(vector<string> &final_input);
	bool check_valid_par(vector<string> &input);

    private:
	char** create_charstar(const string & input);
	void print_charstar(char** input);
	void print_vector(const vector<string> &vec);	
};

#endif //__Parser_HPP__
