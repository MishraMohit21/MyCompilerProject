#include "lexer.h"



void Lexer::print_Token(Token token){

    std::cout << "TOKEN VALUE: '" << token.value << "', ";
    switch (token.type){

        case INT:
          std::cout << "TOKEN TYPE : INT\n";  
          break;
        case KEYWORD:
          std::cout << "TOKEN TYPE : KEYWORD\n";  
          break;
        case SEPERATOR:
          std::cout << "TOKEN TYPE : SEPERATOR\n";  
          break;
        case END_OF_TOKEN:
          std::cout << "END_OF_TOKEN\n";  
          break;
        case NOT_TOKEN:
          std::cout << "NOT A TOKEN\n";
          break;
    }

}


Token Lexer::generate_number(std::string code, int &current_index){

    Token token;
    token.type = INT;
    std::string value;
    while (isdigit(code[current_index]) && code[current_index] != '\0')
    {
        if (!isdigit(code[current_index]))
            break;
        
        value.push_back(code[current_index]);
        current_index++;
    }
    token.value = value;
    return token;
}


Token Lexer::generate_Keyword(std::string code, int &current_index){

    Token token;
    std::string value;
    while (isalpha(code[(current_index)]) && code[current_index] != '\0'){

        value.push_back(code[current_index]);
        current_index++;

    }

    if (value == "exit"){
        token.type = KEYWORD;
        token.value = "EXIT";
    }else if (value == "while"){
        token.type = KEYWORD;
        token.value = "WHILE";
    }else if (value == "for"){
        token.type = KEYWORD;
        token.value = "FOR";
    }else{
        token.type = NOT_TOKEN;
        token.value = value;
    }

    return token;

}

Token Lexer::generate_Seperator(std::string code, int &current_index){

    Token token;
    std::string value;
    value = code[current_index];
    token.type = SEPERATOR;
    token.value = value;
    return token;

}

std::vector<Token> Lexer::lexer_start(std::string code){

    std::vector<Token> Token_array;
    int current_index = 0;
    Token token;
    while(code[current_index] != '\0'){

        if (code[current_index] == ';' || code[current_index] == '(' || code[current_index] == ')'){

            token = generate_Seperator(code, current_index);
            Token_array.push_back(token);
    
        }
        else if (isdigit(code[current_index]))
        {

            token = generate_number(code, current_index);
            Token_array.push_back(token);
            current_index--;

        }
        else if (isalpha(code[current_index]))
        {

            token = generate_Keyword(code, current_index);
            Token_array.push_back(token);
            current_index--;

        }

        current_index++;

    }

    token.value = '\0';
    token.type = END_OF_TOKEN;
    Token_array.push_back(token);

    return Token_array;
}


void Lexer::StartLexing(std::string code)
{

    std::vector<Token> TokenArray;

    TokenArray = lexer_start(code);

    for (auto& tkn : TokenArray)
    {
        print_Token(tkn);
    }

}