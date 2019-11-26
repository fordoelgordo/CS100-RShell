#ifndef __Factory_HPP__
#define __Factory_HPP__

#include "parser.hpp"

using namespace std;

class Factory : public Parser {
    public:
	Factory();
	ExecuteCommand* create_command(string command, string sep);
};

#endif //__Factory_HPP__
