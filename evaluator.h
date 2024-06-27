#pragma once

#include "envm.h"
#include "ast.h"

std::any evaluate(ASTNode* exp, Environment::Ptr env);
