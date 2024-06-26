#include "parser.h"
#include <iostream>
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current_token_index(0) {}

Token Parser::getNextToken() {
    if (current_token_index < tokens.size()) {
        return tokens[current_token_index++];
    }
    return {END_OF_TOKEN, ""};
}

Token Parser::peekNextToken() {
    if (current_token_index < tokens.size()) {
        return tokens[current_token_index];
    }
    return {END_OF_TOKEN, ""};
}

ASTNode* Parser::parse() {
    return parseProgram();
}

ASTNode* Parser::parseProgram() {
    ProgramNode* programNode = new ProgramNode();
    while (peekNextToken().type != END_OF_TOKEN) {
        programNode->prog.push_back(parseStatement());
    }
    return programNode;
}

ASTNode* Parser::parseStatement() {
    Token token = peekNextToken();
    if (token.type == Keyword && token.value == "if") {
        return parseIf();
    } else if (token.type == Keyword && token.value == "func") {
        return parseFunction();
    } else if (token.type == VARIABLE) {
        return parseAssignment();
    } else {
        return parseExpression();
    }
}

ASTNode* Parser::parseExpression() {
    ASTNode* node = parseTerm();
    while (true) {
        Token token = peekNextToken();
        if (token.type == OPERATOR && (token.value == "+" || token.value == "-")) {
            getNextToken();
            BinaryNode* binaryNode = new BinaryNode();
            binaryNode->operator_t = token.value;
            binaryNode->left = node;
            binaryNode->right = parseTerm();
            node = binaryNode;
        } else {
            break;
        }
    }
    return node;
}

ASTNode* Parser::parseTerm() {
    ASTNode* node = parseFactor();
    while (true) {
        Token token = peekNextToken();
        if (token.type == OPERATOR && (token.value == "*" || token.value == "/")) {
            getNextToken();
            BinaryNode* binaryNode = new BinaryNode();
            binaryNode->operator_t = token.value;
            binaryNode->left = node;
            binaryNode->right = parseFactor();
            node = binaryNode;
        } else {
            break;
        }
    }
    return node;
}

ASTNode* Parser::parseFactor() {
    Token token = peekNextToken();
    if (token.type == NUMBER) {
        getNextToken();
        return new NumNode(std::stoi(token.value));
    } else if (token.type == STRING) {
        getNextToken();
        return new StringNode(token.value);
    } else if (token.type == BOOLL) {
        getNextToken();
        return new BoolNode(token.value == "true");
    } else if (token.type == LPAERN) {
        getNextToken();
        ASTNode* node = parseExpression();
        if (peekNextToken().type != RPAREN) {
            throw std::runtime_error("Expected closing parenthesis");
        }
        getNextToken();
        return node;
    } else if (token.type == VARIABLE) {
        return parsePrimary();
    } else {
        throw std::runtime_error("Unexpected token: " + token.value);
    }
}

ASTNode* Parser::parseIf() {
    getNextToken(); // Consume 'if'
    IfNode* ifNode = new IfNode();
    ifNode->condition = parseExpression();
    if (peekNextToken().type != Keyword || peekNextToken().value != "then") {
        throw std::runtime_error("Expected 'then'");
    }
    getNextToken(); // Consume 'then'
    ifNode->then = parseStatement();
    if (peekNextToken().type == Keyword && peekNextToken().value == "else") {
        getNextToken(); // Consume 'else'
        ifNode->Else = parseStatement();
    }
    return ifNode;
}

ASTNode* Parser::parseFunction() {
    getNextToken(); // Consume 'func'
    FunctionNode* funcNode = new FunctionNode();
    if (peekNextToken().type != VARIABLE) {
        throw std::runtime_error("Expected function name");
    }
    getNextToken(); // Consume function name
    if (peekNextToken().type != LPAERN) {
        throw std::runtime_error("Expected '('");
    }
    getNextToken(); // Consume '('
    while (peekNextToken().type != RPAREN) {
        if (peekNextToken().type == VARIABLE) {
            funcNode->vars.push_back(getNextToken().value);
            if (peekNextToken().type == COMM) {
                getNextToken(); // Consume ','
            }
        } else {
            throw std::runtime_error("Expected variable name or ')'");
        }
    }
    getNextToken(); // Consume ')'
    funcNode->body = parseStatement();
    return funcNode;
}

ASTNode* Parser::parseCall() {
    CallNode* callNode = new CallNode();
    callNode->func = parsePrimary();
    if (peekNextToken().type != LPAERN) {
        throw std::runtime_error("Expected '('");
    }
    getNextToken(); // Consume '('
    while (peekNextToken().type != RPAREN) {
        callNode->args.push_back(parseExpression());
        if (peekNextToken().type == COMM) {
            getNextToken(); // Consume ','
        }
    }
    getNextToken(); // Consume ')'
    return callNode;
}

ASTNode* Parser::parseAssignment() {
    VarNode* varNode = new VarNode(getNextToken().value);
    if (peekNextToken().type != OPERATOR || peekNextToken().value != "=") {
        throw std::runtime_error("Expected '='");
    }
    getNextToken(); // Consume '='
    AssignNode* assignNode = new AssignNode();
    assignNode->left = varNode;
    assignNode->right = parseExpression();
    return assignNode;
}

ASTNode* Parser::parsePrimary() {
    Token token = getNextToken();
    if (token.type == VARIABLE) {
        return new VarNode(token.value);
    } else {
        throw std::runtime_error("Unexpected token: " + token.value);
    }
}

void Parser::printAST(ASTNode* node, int level) {
    if (!node) return;

    for (int i = 0; i < level; ++i) std::cout << "  ";
    switch (node->type) {
        case PROGRAM:
            std::cout << "Program\n";
            for (auto& stmt : static_cast<ProgramNode*>(node)->prog) {
                printAST(stmt, level + 1);
            }
            break;
        case ASSIGN:
            std::cout << "Assignment\n";
            printAST(static_cast<AssignNode*>(node)->left, level + 1);
            printAST(static_cast<AssignNode*>(node)->right, level + 1);
            break;
        case CALL:
            std::cout << "Call\n";
            printAST(static_cast<CallNode*>(node)->func, level + 1);
            for (auto& arg : static_cast<CallNode*>(node)->args) {
                printAST(arg, level + 1);
            }
            break;
        case FUNC:
            std::cout << "Function\n";
            for (auto& var : static_cast<FunctionNode*>(node)->vars) {
                for (int i = 0; i < level + 1; ++i) std::cout << "  ";
                std::cout << "Var: " << var << "\n";
            }
            printAST(static_cast<FunctionNode*>(node)->body, level + 1);
            break;
        case BINARY:
            std::cout << "Binary Operation: " << static_cast<BinaryNode*>(node)->operator_t << "\n";
            printAST(static_cast<BinaryNode*>(node)->left, level + 1);
            printAST(static_cast<BinaryNode*>(node)->right, level + 1);
            break;
        case VAR:
            std::cout << "Variable: " << static_cast<VarNode*>(node)->value << "\n";
            break;
        case NUM:
            std::cout << "Number: " << static_cast<NumNode*>(node)->value << "\n";
            break;
        case STR:
            std::cout << "String: " << static_cast<StringNode*>(node)->value << "\n";
            break;
        case Bool:
            std::cout << "Boolean: " << (static_cast<BoolNode*>(node)->value ? "true" : "false") << "\n";
            break;
        case If:
            std::cout << "If Statement\n";
            std::cout << "Condition:\n";
            printAST(static_cast<IfNode*>(node)->condition, level + 1);
            std::cout << "Then:\n";
            printAST(static_cast<IfNode*>(node)->then, level + 1);
            std::cout << "Else:\n";
            printAST(static_cast<IfNode*>(node)->Else, level + 1);
            break;
        default:
            std::cout << "Unknown Node\n";
            break;
    }
}
