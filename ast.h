#pragma once

#include <vector>
#include <string>
#include <iostream>


enum NodeType {
    PROGRAM, ASSIGN, CALL, LAMDA, BINARY, VAR, NUM
};

struct ASTNode
{
    NodeType type;
    virtual ~ASTNode() = default;
};

struct ProgramNode : public ASTNode{
    std::vector<ASTNode*> prog;
    ProgramNode() {type = PROGRAM;}
};


struct AssignNode : public ASTNode{
    std::string operator_t;
    ASTNode* left;
    ASTNode* right;
    AssignNode() {type = ASSIGN;}
};

struct CallNode : public ASTNode{
    ASTNode* func;
    std::vector<ASTNode*> args;
    CallNode() {type = CALL;}
};

struct LamdaNode : public ASTNode{
    std::vector<std::string> vars;
    ASTNode* body;
    LamdaNode() {type = LAMDA;}
};

struct BinaryNode : public ASTNode{
    
    std::string operator_t;
    ASTNode* left;
    ASTNode* right;
    BinaryNode() {type = BINARY;}
};


struct VarNode : public ASTNode{
    std::string value;
    VarNode(const std::string& val) : value(val) {type = VAR;}
};

struct NumNode : public ASTNode{
    int value;
    NumNode(int val) : value(val) {type = NUM;}
};
