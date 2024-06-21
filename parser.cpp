#include "parser.h"
#include <iostream>
#include <stdexcept>
#include <vector>


Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), currentTokenIndex(0) {
    std::cout << "Initializing Parser with tokens\n";
}


ASTNode* Parser::parse() {
    std::cout << "Starting parse\n";
    return parseProgram();
}


void Parser::printAST(ASTNode* node, int indent) {
    if (!node) return;
    for (int i = 0; i < indent; ++i) std::cout << "  ";

    switch (node->type) {
        case PROGRAM: {
            std::cout << "Program\n";
            ProgramNode* progNode = static_cast<ProgramNode*>(node);
            for (ASTNode* stmt : progNode->prog) {
                printAST(stmt, indent + 1);
            }
            break;
        }
        case ASSIGN: {
            std::cout << "Assign\n";
            AssignNode* assignNode = static_cast<AssignNode*>(node);
            printAST(assignNode->left, indent + 1);
            printAST(assignNode->right, indent + 1);
            break;
        }
        case CALL: {
            std::cout << "Call\n";
            CallNode* callNode = static_cast<CallNode*>(node);
            printAST(callNode->func, indent + 1);
            for (ASTNode* arg : callNode->args) {
                printAST(arg, indent + 1);
            }
            break;
        }
        case FUNC: {
            std::cout << "Function\n";
            FunctionNode* funcNode = static_cast<FunctionNode*>(node);
            for (const std::string& var : funcNode->vars) {
                for (int i = 0; i < indent + 1; ++i) std::cout << "  ";
                std::cout << "Var: " << var << "\n";
            }
            printAST(funcNode->body, indent + 1);
            break;
        }
        case BINARY: {
            std::cout << "Binary\n";
            BinaryNode* binaryNode = static_cast<BinaryNode*>(node);
            std::cout << binaryNode->operator_t << "\n";
            printAST(binaryNode->left, indent + 1);
            printAST(binaryNode->right, indent + 1);
            break;
        }
        case VAR: {
            VarNode* varNode = static_cast<VarNode*>(node);
            std::cout << "Var: " << varNode->value << "\n";
            break;
        }
        case NUM: {
            NumNode* numNode = static_cast<NumNode*>(node);
            std::cout << "Num: " << numNode->value << "\n";
            break;
        }
        case Bool: {
            BoolNode* boolNode = static_cast<BoolNode*>(node);
            std::cout << "Bool: " << (boolNode->value ? "true" : "false") << "\n";
            break;
        }
        case STR: {
            StringNode* strNode = static_cast<StringNode*>(node);
            std::cout << "String: " << strNode->value << "\n";
            break;
        }
        case If: {
            IfNode* ifNode = static_cast<IfNode*>(node);
            std::cout << "If\n";
            printAST(&ifNode->condition, indent + 1);
            printAST(&ifNode->then, indent + 1);
            printAST(&ifNode->Else, indent + 1);
            break;
        }
        default:
            std::cout << "Unknown node type\n";
            break;
    }
}


ASTNode* Parser::parseProgram() {
    std::cout << "Parsing program\n";
    ProgramNode* program = new ProgramNode();
    while (currentTokenIndex < tokens.size() && tokens[currentTokenIndex].type != END_OF_TOKEN) {
        program->prog.push_back(parseStatement());
    }
    return program;
}


ASTNode* Parser::parseStatement() {
    std::cout << "Parsing statement\n";
    Token token = peekNextToken();
    if (token.type == Keyword) {
        if (token.value == "if") {
            return parseIf();
        } else if (token.value == "func") {
            return parseFunction();
        }
    } else if (token.type == VARIABLE) {
        Token nextToken = peekNextToken(1);
        if (nextToken.type == OPERATOR && nextToken.value == "=") {
            return parseAssign();
        } else {
            return parseExpression();
        }
    }
    return nullptr;
}


ASTNode* Parser::parseExpression() {
    std::cout << "Parsing expression\n";
    Token token = peekNextToken();
    if (token.type == OPERATOR) {
        return parseBinary();
    } else if (token.type == DATATYPE) {
        if (std::isdigit(token.value[0])) {
            return parseNum();
        } else if (token.value[0] == '"') {
            return parseString();
        } else if (token.value == "true" || token.value == "false") {
            return parseBool();
        }
    } else if (token.type == VARIABLE) {
        return parseVar();
    }
    return nullptr;
}


ASTNode* Parser::parseAssign() {
    std::cout << "Parsing assignment\n";
    ASTNode* left = parseVar();
    expect(OPERATOR, "=");
    ASTNode* right = parseExpression();
    AssignNode* assign = new AssignNode();
    assign->left = left;
    assign->right = right;
    return assign;
}


ASTNode* Parser::parseCall() {
    std::cout << "Parsing function call\n";
    Token funcToken = getNextToken();
    CallNode* call = new CallNode();
    call->func = new VarNode(funcToken.value);
    expect(SEPERATOR, "(");
    while (peekNextToken().type != SEPERATOR || peekNextToken().value != ")") {
        call->args.push_back(parseExpression());
        if (peekNextToken().type == SEPERATOR && peekNextToken().value == ",") {
            expect(SEPERATOR, ",");
        }
    }
    expect(SEPERATOR, ")");
    return call;
}


ASTNode* Parser::parseFunction() {
    std::cout << "Parsing function\n";
    expect(Keyword, "func");
    FunctionNode* func = new FunctionNode();
    expect(SEPERATOR, "(");
    while (peekNextToken().type != SEPERATOR || peekNextToken().value != ")") {
        func->vars.push_back(getNextToken().value);
        if (peekNextToken().type == SEPERATOR && peekNextToken().value == ",") {
            expect(SEPERATOR, ",");
        }
    }
    expect(SEPERATOR, ")");
    expect(SEPERATOR, "(");
    func->body = parseStatement();
    expect(SEPERATOR, ")");
    return func;
}


ASTNode* Parser::parseBinary() {
    std::cout << "Parsing binary expression\n";
    ASTNode* left = parseExpression();
    std::string op = getNextToken().value;
    ASTNode* right = parseExpression();
    BinaryNode* binary = new BinaryNode();
    binary->left = left;
    binary->right = right;
    binary->operator_t = op;
    return binary;
}


ASTNode* Parser::parseVar() {
    std::cout << "Parsing variable\n";
    std::string value = getNextToken().value;
    return new VarNode(value);
}


ASTNode* Parser::parseNum() {
    std::cout << "Parsing number\n";
    int value = std::stoi(getNextToken().value);
    return new NumNode(value);
}


ASTNode* Parser::parseBool() {
    std::cout << "Parsing boolean\n";
    std::string value = getNextToken().value;
    return new BoolNode(value == "true");
}


ASTNode* Parser::parseString() {
    std::cout << "Parsing string\n";
    std::string value = getNextToken().value;
    return new StringNode(value);
}


ASTNode* Parser::parseIf() {
    std::cout << "Parsing if statement\n";
    expect(Keyword, "if");
    ASTNode* condition = parseExpression();
    expect(SEPERATOR, "(");
    ASTNode* thenStmt = parseStatement();
    expect(SEPERATOR, ")");
    ASTNode* elseStmt = nullptr;
    if (peekNextToken().value == "else") {
        expect(Keyword, "else");
        expect(SEPERATOR, "(");
        elseStmt = parseStatement();
        expect(SEPERATOR, ")");
    }
    return new IfNode(*condition, *thenStmt, *elseStmt);
}


Token Parser::getNextToken() {
    std::cout << "Getting next token\n";
    if (currentTokenIndex < tokens.size()) {
        return tokens[currentTokenIndex++];
    }
    throw std::runtime_error("Unexpected end of input");
}


Token Parser::peekNextToken(int offset) {
    std::cout << "Peeking next token with offset " << offset << "\n";
    if (currentTokenIndex + offset < tokens.size()) {
        return tokens[currentTokenIndex + offset];
    }
    throw std::runtime_error("Unexpected end of input");
}


void Parser::expect(TokenType expectedType, const std::string& expectedValue) {
    std::cout << "Expecting token of type " << expectedType << " with value " << expectedValue << "\n";
    Token token = getNextToken();
    if (token.type != expectedType || token.value != expectedValue) {
        throw std::runtime_error("Expected token type: " + std::to_string(expectedType) + " with value: " + expectedValue + ", but got: " + token.value);
    }
}


void Parser::expect(TokenType expectedType) {
    std::cout << "Expecting token of type " << expectedType << "\n";
    Token token = getNextToken();
    if (token.type != expectedType) {
        throw std::runtime_error("Expected token type: " + std::to_string(expectedType) + ", but got: " + std::to_string(token.type));
    }
}
