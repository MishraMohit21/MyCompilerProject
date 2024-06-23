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

void typePrinter(ASTNode* node){
    switch (node->type) {
        case PROGRAM: {
            std::cout << "Program\n";
            break;
        }
        case ASSIGN: {
            std::cout << "Assign\n";
            break;
        }
        case CALL: {
            std::cout << "Call\n";
            break;
        }
        case FUNC: {
            std::cout << "Function\n";
            break;
        }
        case BINARY: {
            std::cout << "Binary\n";
            break;
        }
        case VAR: {
            std::cout << "VAR\n";
            break;
        }
        case NUM: {
            std::cout << "NUM\n";
            break;
        }
        case Bool: {
            std::cout << "Bool\n";
            break;
        }
        case STR: {
            std::cout << "STR\n";
            break;
        }
        case If: {
            std::cout << "IF\n";
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
    while (tokens[currentTokenIndex].type != END_OF_TOKEN) {
        std::cout << "Current token index: " << currentTokenIndex << std::endl;
        ASTNode* returnNode = parseStatement();
        if (returnNode) {
            typePrinter(returnNode);
            program->prog.push_back(returnNode);
        }
        else {
            // Error or no valid statement, break to avoid infinite loop
            std::cout << "Error or no valid statement at index: " << currentTokenIndex << std::endl;
            break;
        }
    }
    return program;
}

bool is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

ASTNode* Parser::parseStatement() {
    std::cout << "Parsing statement\n";
    Token token = peekNextToken();
    std::cout << "Current token: type = " << token.type << ", value = " << token.value << "\n";
    if (token.type == Keyword) {
        if (token.value == "if") {
            return parseIf();
        } else if (token.value == "func") {
            return parseFunction();
        }
    } else if (token.type == VARIABLE) {
        Token nextToken = peekNextToken(1);
        std::cout << "Next token: type = " << nextToken.type << ", value = " << nextToken.value << "\n";
        if (nextToken.type == OPERATOR && nextToken.value == "=") {
            return parseAssign();
        } else {
            return parseExpression();
        }
    }
    //getNextToken(); // Consume the token to prevent infinite loop
    return nullptr;
}

ASTNode* Parser::parseExpression() {
    std::cout << "Parsing expression\n";
    Token token = peekNextToken();
    std::cout << "Current token: type = " << token.type << ", value = " << token.value << "\n";

    if (token.type == SEPERATOR && token.value == "(") {
        getNextToken(); // Consume '('
        ASTNode* expr = parseExpression();
        expect(SEPERATOR, ")");
        return expr;
    }
    else if (token.type == OPERATOR) {
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
    getNextToken(); // Consume the token to prevent infinite loop
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
    func->body = parseStatement();
    return func;
}

ASTNode* Parser::parseBinary() {
    std::cout << "Parsing binary expression\n";
    Token leftToken = getNextToken();
    ASTNode* left = new VarNode(leftToken.value);
    Token opToken = getNextToken();
    Token rightToken = getNextToken();
    ASTNode* right = new VarNode(rightToken.value);
    BinaryNode* binary = new BinaryNode();
    binary->left = left;
    binary->operator_t = opToken.value;
    binary->right = right;
    return binary;
}

ASTNode* Parser::parseVar() {
    std::cout << "Parsing variable\n";
    Token token = getNextToken();
    return new VarNode(token.value);
}

ASTNode* Parser::parseNum() {
    std::cout << "Parsing number\n";
    Token token = getNextToken();
    return new NumNode(std::stoi(token.value));
}

ASTNode* Parser::parseBool() {
    std::cout << "Parsing boolean\n";
    Token token = getNextToken();
    return new BoolNode(token.value == "true");
}

ASTNode* Parser::parseString() {
    std::cout << "Parsing string\n";
    Token token = getNextToken();
    return new StringNode(token.value.substr(1, token.value.size() - 2)); // Remove quotes
}

ASTNode* Parser::parseIf() {
    std::cout << "Parsing if statement\n";
    expect(Keyword, "if");
    IfNode* ifNode = new IfNode();
    ifNode->condition = *parseExpression();
    ifNode->then = *parseStatement();
    if (peekNextToken().type == Keyword && peekNextToken().value == "else") {
        getNextToken(); // Consume 'else'
        ifNode->Else = *parseStatement();
    }
    return ifNode;
}

Token Parser::getNextToken() {
    if (currentTokenIndex < tokens.size()) {
        return tokens[currentTokenIndex++];
    }
    return Token{ END_OF_TOKEN, "" };
}

Token Parser::peekNextToken(int offset) const {
    if (currentTokenIndex + offset < tokens.size()) {
        return tokens[currentTokenIndex + offset];
    }
    return Token{ END_OF_TOKEN, "" };
}

void Parser::expect(TokenType type, const std::string& value) {
    Token token = getNextToken();
    if (token.type != type || token.value != value) {
        std::cerr << "Expected token type " << type << " with value " << value << ", but got type " << token.type << " with value " << token.value << "\n";
        throw std::runtime_error("Unexpected token");
    }
}
