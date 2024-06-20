#include "lexer.h"



void Lexer::print_Token(Token token){

    std::cout << "TOKEN VALUE: '" << token.value << "', ";
    switch (token.type){

        case NUMBER:
          std::cout << "TOKEN TYPE : NUMBER\n";  
          break;
        case PRINT:
          std::cout << "TOKEN TYPE : PRINT\n";  
          break;
        case SEPERATOR:
          std::cout << "TOKEN TYPE : SEPERATOR\n";  
          break;
        case VARIABLE:
            std::cout << "TOKEN TYPE : VARIABLE\n";
            break;
        case OPERATOR:
            std::cout << "TOKEN TYPE : OPERATOR\n";
            break;
        case STRING:
            std::cout << "TOKEN TYPE : STRING\n";
            break;
        case EXIT: 
            std::cout << "TOKEN TYPE : EXIT\n";
            break;
        case FUNCTION:
            std::cout << "TOKEN TYPE : FUNCTION\n"
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
    token.type = NUMBER;
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

Token Lexer::generate_Operator(std::string code, int &current_index){

    Token token;
    std::string operators = "+-*/%=&|<>!";
    
    // Check if the current character is an operator
    if (current_index < code.length() && operators.find(code[current_index]) != std::string::npos) {
        token.type = OPERATOR;
        token.value.push_back(code[current_index]);
    } else {
        token.type = NOT_TOKEN;
    }

    return token;
}


Token Lexer::generate_Keyword(std::string code, int &current_index){

    int start = current_index;
    Token token;
    std::string value;
    while (isalpha(code[(current_index)]) && code[current_index] != '\0'){

        value.push_back(code[current_index]);
        current_index++;

    }

    if (value == "exit"){
        token.type = EXIT;
        token.value = "EXIT";
    }else if (value == "println")
    {
        token.type = PRINT;
        token.value = value;
    }else if (value == "func"){
        token.type = FUNCTION;
        token.value = value;
    }
    else{
        token = generate_Variable(code, start);
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


// Function to check if a character can be the start of an identifier
bool isIdStart(char ch) {
    return std::isalpha(ch) || ch == '_';
}

// Function to check if a character can be part of an identifier
bool isId(char ch) {
    return std::isalnum(ch) || ch == '_';
}


Token Lexer::generate_Variable(std::string code, int &current_index){

    Token token;
    token.type = NOT_TOKEN;
    
    if (isIdStart(code[current_index])) {
        size_t start = current_index;
        while (current_index < code.length() && isId(code[current_index])) {
            ++current_index;
        }
        token.type = VARIABLE;
        token.value = code.substr(start, current_index - start);
    }

    return token;

}

Token Lexer::generate_String(std::string code, int &current_index){

    Token token; 
    token.type = STRING;
    std::string value;
    while (code[current_index] != '"' && code[current_index - 1] != (char)92){

        value.push_back(code[current_index]);
        current_index++;

    }

    token.value = value;
    return token;
}


std::vector<Token> Lexer::lexer_start(std::string code){

    const std::string operators = "+-*/%=&|<>!";

    std::vector<Token> Token_array;
    int current_index = 0;
    Token token;
    token.type = NOT_TOKEN;
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
        else if( operators.find(code [current_index]) != std::string::npos)
        {
            token = generate_Operator(code, current_index);
            Token_array.push_back(token);
        }
        else if (code[current_index] == '"'){
            current_index++;
            token = generate_String(code, current_index);
            Token_array.push_back(token);
        }

        current_index++;

    }

    token.value = '\0';
    token.type = END_OF_TOKEN;
    Token_array.push_back(token);

    for (auto& tkn : Token_array)
    {
        print_Token(tkn);
    }

    return Token_array;
}


