#include "evaluator.h"

std::any evaluate(ASTNode* exp, Environment::Ptr env) {
    switch (exp->type) {
        case NUM: {
            NumNode* numNode = static_cast<NumNode*>(exp);
            return numNode->value;
        }
        case STR: {
            StringNode* strNode = static_cast<StringNode*>(exp);
            return strNode->value;
        }
        case Bool: {
            BoolNode* boolNode = static_cast<BoolNode*>(exp);
            return boolNode->value;
        }
        case VAR: {
            VarNode* varNode = static_cast<VarNode*>(exp);
            return env->get(varNode->value);
        }
        case ASSIGN: {
            AssignNode* assignNode = static_cast<AssignNode*>(exp);
            if (assignNode->left->type != VAR) {
                throw std::runtime_error("Cannot assign to non-variable");
            }
            VarNode* varNode = static_cast<VarNode*>(assignNode->left);
            std::any rightValue = evaluate(assignNode->right, env);
            env->set(varNode->value, rightValue);
            return rightValue;
        }
        case BINARY: {
            BinaryNode* binaryNode = static_cast<BinaryNode*>(exp);
            std::any leftValue = evaluate(binaryNode->left, env);
            std::any rightValue = evaluate(binaryNode->right, env);
            // Implement apply_op here or call a separate function
        }
        case FUNC: {
            FunctionNode* funcNode = static_cast<FunctionNode*>(exp);
            // Implement make_lambda here or call a separate function
        }
        case If: {
            IfNode* ifNode = static_cast<IfNode*>(exp);
            std::any condValue = evaluate(ifNode->condition, env);
            if (std::any_cast<bool>(condValue)) {
                return evaluate(ifNode->then, env);
            } else if (ifNode->Else->type != NodeType::Bool) {
                return evaluate(ifNode->Else, env);
            }
            return false;
        }
        case PROGRAM: {
            ProgramNode* progNode = static_cast<ProgramNode*>(exp);
            std::any val = false;
            for (ASTNode* node : progNode->prog) {
                val = evaluate(node, env);
            }
            return val;
        }
        case CALL: {
            CallNode* callNode = static_cast<CallNode*>(exp);
            std::function<std::any(std::vector<std::any>)> func = std::any_cast<std::function<std::any(std::vector<std::any>)>>(evaluate(callNode->func, env));
            std::vector<std::any> args;
            for (ASTNode* arg : callNode->args) {
                args.push_back(evaluate(arg, env));
            }
            return func(args);
        }
        default:
            throw std::runtime_error("Unknown expression type");
    }
}
