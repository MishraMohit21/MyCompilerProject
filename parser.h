#pragma once

#include "lexer.h"
#include "ast.h"
#include <vector>

class Parser {
    std::vector<Token> tokens;
    int current_token_index;
    Token getNextToken();
    Token peekNextToken();

    ASTNode* parseProgram();
    ASTNode* parseStatement();
    ASTNode* parseExpression();
    ASTNode* parseTerm();
    ASTNode* parseFactor();
    ASTNode* parseIf();
    ASTNode* parseFunction();
    ASTNode* parseCall();
    ASTNode* parseAssignment();
    ASTNode* parsePrintln();
    ASTNode* parsePrimary();

public:
    Parser(const std::vector<Token>& tokens);
    ASTNode* parse();
    void printAST(ASTNode* node, int level = 0);
};
