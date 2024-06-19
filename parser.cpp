#include "Parser.h"



void parser(std::vector<Token> &Tokens)
{

    Token *current_token = &Tokens[0];

    while (current_token->type != END_OF_TOKEN){
        std::cout << "123\n";
        current_token++;
    }

}
