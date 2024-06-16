#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <sstream>

typedef enum {

    INT, 
    KEYWORD,
    SEPERATOR,
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
    std::vector<Token> lexer_start(std::string code);

public:
    
    void StartLexing(std::string code);
        

};


