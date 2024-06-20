#include "parser.h"


Token Parser::getNextToken(){
    if (current_index < tokens.size()){
        return tokens[current_index++];
    }
    return {END_OF_TOKEN, ""};
}

Token Parser::peekNextToken() const {
    if (current_index < tokens.size()){
        return tokens[current_index];
    }
    return {END_OF_TOKEN, ""};
}

void Parser::reportError(const std::string& errorMessage){
    std::cerr << "Syntax Error at Token " << current_index << ": " << errorMessage << std::endl;
}

ProgramNode* Parser::parseProgram() {
     ProgramNode* programNode = new ProgramNode();
        while (current_index < tokens.size()) {
            ASTNode* statement = pareseStatement();
            if (!statement) {
                delete programNode;
                return nullptr;
            }
            programNode->prog.push_back(statement);
        }
        return programNode;
}

ASTNode* Parser::pareseStatement(){

    Token next = peekNextToken();
    if (next.type == )
}

