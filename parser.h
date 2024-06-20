#include "lexer.h"
#include "ast.h"

class Parser {

    std::vector <Token> tokens;
    int current_index;

    Token getNextToken();
    Token peekNextToken() const;
    void reportError (const std::string& errorMessage);

public:

    Parser(const std::vector<Token> & tokens) : tokens(tokens), current_index(0) {}

    ProgramNode* parseProgram();
    ASTNode* pareseStatement();
    ASTNode* parseAssign();
    ASTNode* parseCall();
    ASTNode* parseExpression();
    ASTNode* parseTerm();
    ASTNode* parseFactor();
    
};