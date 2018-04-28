#ifndef RULES_H_INCLUDED
#define RULES_H_INCLUDED
#include "token.h"

typedef enum {FUNCTION_PARAM, FUNCTION_DEFINITION, VARIABLE_DEFINITION, SIMPLE_TOKEN} RuleType;

typedef struct FunctionParam
{
    Token* dataType;
    Token* identifier;
} FunctionParam;

typedef struct FunctionDefinition
{
    Token* dataType;
    Token* identifier;
    FunctionParam* paramList;
} FunctionDefinition;

typedef struct VariableDefinition
{
    Token* dataType;
    Token* tokenList;
} VariableDefinition;

typedef struct GenericSyntaxTreeNode
{
    int stateToSee;
    RuleType ruleType;
    union
    {
        VariableDefinition* varDef;
        FunctionDefinition* functionDef;
        FunctionParam* functionParam;
        Token* token;
    } attr;
} GenericSyntaxTreeNode;


#endif // RULES_H_INCLUDED
