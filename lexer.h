#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <sstream>

typedef enum {

    INT, 
    OPERATOR,
    KEYWORD,
    SEPERATOR,
    VARIABLE,
    STRING,
    NOT_TOKEN,
    END_OF_TOKEN,

} TokenType;

typedef struct {

    TokenType type;
    std::string value;

} Token;

class Lexer {

    void print_Token(Token token);
    Token generate_number(std::string code, int &current_index);
    Token generate_Keyword(std::string code, int &current_index);
    Token generate_Seperator(std::string code, int &current_index);
    Token generate_Operator(std::string code, int &current_index);
    Token generate_Variable(std::string code, int &current_index);
    Token generate_String(std::string code, int &current_index);

public:
    
    std::vector<Token> lexer_start(std::string code);

};


