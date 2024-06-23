#pragma once

#include <vector>
#include <string>
#include <iostream>


enum NodeType {
    PROGRAM, ASSIGN, CALL, FUNC, BINARY, VAR, NUM,  STR, Bool, If, SEP,
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
    std::string operator_t = " = ";
    ASTNode* left;
    ASTNode* right;
    AssignNode() {type = ASSIGN;}
};

struct CallNode : public ASTNode{
    ASTNode* func;
    std::vector<ASTNode*> args;
    CallNode() {type = CALL;}
};

struct FunctionNode : public ASTNode{
    std::vector<std::string> vars;
    ASTNode* body;
    FunctionNode() {type = FUNC;}
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

struct BoolNode : public ASTNode
{   
    bool value;
    BoolNode(bool val) : value (val) {type = Bool;}
};

struct StringNode : public ASTNode
{
    std::string value;
    StringNode(std::string val) : value (val) {type = STR;}
};

struct IfNode : public ASTNode
{
    ASTNode condition , then, Else;
    IfNode(ASTNode cond, ASTNode th, ASTNode El) : condition(cond), then (th), Else (El) {type = If;}
    IfNode() {type = If;}

};


