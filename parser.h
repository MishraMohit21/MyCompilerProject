#pragma once
#include "lexer.h"

typedef enum {

    INT_LIT,
    STATEMENT,
    EXTRA,
    BEGINNING,

}NodeTypes;

typedef struct Node{

    std::string value;
    NodeTypes type;
    struct Node *right = NULL;
    struct Node *left = NULL;

}Node;


void print_Tree(Node *node);
void init_node(Node *node, std::string value, NodeTypes type);
void parser(std::vector<Token> tokens);

