#pragma once

#include "AST.h"
#include "lexer.h"

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    ASTNode* parse();
    void printAST(ASTNode* node, int indent = 0);

private:
    std::vector<Token> tokens;
    size_t currentTokenIndex;

    ASTNode* parseProgram();
    ASTNode* parseStatement();
    ASTNode* parseExpression();
    ASTNode* parseAssign();
    ASTNode* parseCall();
    ASTNode* parseFunction();
    ASTNode* parseBinary();
    ASTNode* parseVar();
    ASTNode* parseNum();
    ASTNode* parseBool();
    ASTNode* parseString();
    ASTNode* parseIf();

    Token getNextToken();
    // Token peekNextToken();
    void expect(TokenType expectedType);
    void expect(TokenType expectedType, const std::string& expectedValue);
    Token peekNextToken(int offset = 1);
};
