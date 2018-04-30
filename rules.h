#ifndef RULES_H_INCLUDED
#define RULES_H_INCLUDED
#include "token.h"

typedef enum {INITIAL, PROGRAM, DEFINITIONS, DEFINITIONS_1, DEFINITION, DEFINITION_1, DEF_VAR, VAR_LIST, VAR_LIST_1,
              DEF_FUNC, PARAMETERS, PARAMETERS_1, PARAMS_LIST, PARAMS_LIST_1, FUNC_BLOCK, LOCAL_VARS_DEF,
              LOCAL_VARS_DEF_1, LOCAL_VAR_DEF, LOCAL_VAR_DEF_1, STATEMENTS, STATEMENTS_1, STATEMENT, STATEMENT_1,
              STATEMENT_2, STATEMENT_3, STATEMENT_4, ELSE_PART, ELSE_PART_1, BLOCK, RETURN_VALUE, RETURN_VALUE_1,
              ARGS, ARGS_1, ARG_LIST, ARG_LIST_1, TERM, TERM_1, TERM_2, TERM_3, TERM_4, FUNC_CALL, BLOCK_STM,
              BLOCK_STM_1, EXPRESSION, EXPRESSION_1, EXPRESSION_2, EXPRESSION_3, EXPRESSION_4, EXPRESSION_5,
              EXPRESSION_6, EXPRESSION_7, EXPRESSION_8, EXPRESSION_9, SIMPLE_TOKEN, EPSILON_RULE
             } RuleType;


typedef struct Program
{
    struct GenericSyntaxTreeNode* definitions;
} Program;

typedef struct Definitions
{
    struct GenericSyntaxTreeNode* definition;
    struct GenericSyntaxTreeNode* moreDefinitions;
} Definitions;

typedef struct Definition
{
    struct GenericSyntaxTreeNode* defVar;
} Definition;

typedef struct Definition_1
{
    struct GenericSyntaxTreeNode* defFunc;
} Definition_1;

typedef struct DefVar
{
    struct GenericSyntaxTreeNode* dataType;
    struct GenericSyntaxTreeNode* identifier;
    struct GenericSyntaxTreeNode* varList;
} DefVar;

typedef struct VarList
{
    struct GenericSyntaxTreeNode* identifier;
    struct GenericSyntaxTreeNode* nextVar;
} VarList;

typedef struct DefFunc
{
    struct GenericSyntaxTreeNode* dataType;
    struct GenericSyntaxTreeNode* identifier;
    struct GenericSyntaxTreeNode* params;
    struct GenericSyntaxTreeNode* blockFunc;
} DefFunc;

typedef struct Params
{
    struct GenericSyntaxTreeNode* dataType;
    struct GenericSyntaxTreeNode* identifier;
    struct GenericSyntaxTreeNode* paramList;
} Params;

typedef struct ParamList
{
    struct GenericSyntaxTreeNode* dataType;
    struct GenericSyntaxTreeNode* identifier;
    struct GenericSyntaxTreeNode* paramList;
} ParamList;

typedef struct FuncBlock
{
    struct GenericSyntaxTreeNode* localVarsDef;
} FuncBlock;

typedef struct LocalVarsDef
{
    struct GenericSyntaxTreeNode* localVarDef;
    struct GenericSyntaxTreeNode* localVarsDef;
} LocalVarsDef;

typedef struct LocalVarDef
{
    struct GenericSyntaxTreeNode* defVar;
} LocalVarDef;

typedef struct LocalVarDef_1
{
    struct GenericSyntaxTreeNode* statement;
} LocalVarDef_1;

typedef struct Statements
{
    struct GenericSyntaxTreeNode* statement;
    struct GenericSyntaxTreeNode* statements;
} Statements;

typedef struct Statement
{
    struct GenericSyntaxTreeNode* identifier;
    struct GenericSyntaxTreeNode* equalsSign;
    struct GenericSyntaxTreeNode* expression;
} Statement;

typedef struct Statement_1
{
    struct GenericSyntaxTreeNode* expression;
    struct GenericSyntaxTreeNode* partIf;
    struct GenericSyntaxTreeNode* partElse;
} Statement_1;

typedef struct Statement_2
{
    struct GenericSyntaxTreeNode* expression;
    struct GenericSyntaxTreeNode* block;
} Statement_2;

typedef struct Statement_3
{
    struct GenericSyntaxTreeNode* returnValue;
} Statement_3;

typedef struct Statement_4
{
    struct GenericSyntaxTreeNode* funcCall;
} Statement_4;

typedef struct ElsePart
{
    struct GenericSyntaxTreeNode* blockStm;
} ElsePart;

typedef struct Block
{
    struct GenericSyntaxTreeNode* statements;
} Block;

typedef struct ReturnValue
{
    struct GenericSyntaxTreeNode* expression;
} ReturnValue;

typedef struct Args
{
    struct GenericSyntaxTreeNode* expression;
    struct GenericSyntaxTreeNode* argList;
} Args;

typedef struct ArgList
{
    struct GenericSyntaxTreeNode* expression;
    struct GenericSyntaxTreeNode* argList;
} ArgList;

typedef struct Term
{
    struct GenericSyntaxTreeNode* funcCall;
} Term;

typedef struct Term_1
{
    struct GenericSyntaxTreeNode* identifier;
} Term_1;

typedef struct Term_2
{
    struct GenericSyntaxTreeNode* integer;
} Term_2;

typedef struct Term_3
{
    struct GenericSyntaxTreeNode* realNumber;
} Term_3;

typedef struct Term_4
{
    struct GenericSyntaxTreeNode* constChar;
} Term_4;

typedef struct FuncCall
{
    struct GenericSyntaxTreeNode* identifier;
    struct GenericSyntaxTreeNode* args;
} FuncCall;

typedef struct BlockStm
{
    struct GenericSyntaxTreeNode* statement;
} BlockStm;

typedef struct BlockStm_1
{
    struct GenericSyntaxTreeNode* block;
} BlockStm_1;

typedef struct Expression
{
    struct GenericSyntaxTreeNode* expression;
} Expression;

typedef struct Expression_1
{
    struct GenericSyntaxTreeNode* additionOperator;
    struct GenericSyntaxTreeNode* expression;
} Expression_1;

typedef struct Expression_2
{
    struct GenericSyntaxTreeNode* multOperator;
    struct GenericSyntaxTreeNode* expression;
} Expression_2;

typedef struct Expression_3
{
    struct GenericSyntaxTreeNode* expressionLeft;
    struct GenericSyntaxTreeNode* multOperator;
    struct GenericSyntaxTreeNode* expressionRight;
} Expression_3;

typedef struct Expression_4
{
    struct GenericSyntaxTreeNode* expressionLeft;
    struct GenericSyntaxTreeNode* additionOperator;
    struct GenericSyntaxTreeNode* expressionRight;
} Expression_4;

typedef struct Expression_5
{
    struct GenericSyntaxTreeNode* expressionLeft;
    struct GenericSyntaxTreeNode* relationalOperator;
    struct GenericSyntaxTreeNode* expressionRight;
} Expression_5;

typedef struct Expression_6
{
    struct GenericSyntaxTreeNode* expressionLeft;
    struct GenericSyntaxTreeNode* equalsComparisonOperator;
    struct GenericSyntaxTreeNode* expressionRight;
} Expression_6;

typedef struct Expression_7
{
    struct GenericSyntaxTreeNode* expressionLeft;
    struct GenericSyntaxTreeNode* andOperator;
    struct GenericSyntaxTreeNode* expressionRight;
} Expression_7;

typedef struct Expression_8
{
    struct GenericSyntaxTreeNode* expressionLeft;
    struct GenericSyntaxTreeNode* orOperator;
    struct GenericSyntaxTreeNode* expressionRight;
} Expression_8;

typedef struct Expression_9
{
    struct GenericSyntaxTreeNode* term;
} Expression_9;

typedef struct GenericSyntaxTreeNode
{
    int stateToSee;
    RuleType ruleType;
    union
    {
        Program* program;
        Definitions* definitions;
        Definition* definition;
        Definition_1* definition_1;
        DefVar* defVar;
        VarList* varList;
        DefFunc* defFunc;
        Params* params;
        ParamList* paramList;
        FuncBlock* funcBlock;
        LocalVarsDef* localVarsDef;
        LocalVarDef* localVarDef;
        LocalVarDef_1* localVarDef_1;
        Statements* statements;
        Statement* statement;
        Statement_1* statement_1;
        Statement_2* statement_2;
        Statement_3* statement_3;
        Statement_4* statement_4;
        ElsePart* elsePart;
        Block* block;
        ReturnValue* returnValue;
        Args* args;
        ArgList* argList;
        Term* term;
        Term_1* term_1;
        Term_2* term_2;
        Term_3* term_3;
        Term_4* term_4;
        FuncCall* funcCall;
        BlockStm* blockStm;
        BlockStm_1* blockStm_1;
        Expression* expression;
        Expression_1* expression_1;
        Expression_2* expression_2;
        Expression_3* expression_3;
        Expression_4* expression_4;
        Expression_5* expression_5;
        Expression_6* expression_6;
        Expression_7* expression_7;
        Expression_8* expression_8;
        Expression_9* expression_9;
        Token* simpleToken;
    } attr;
} GenericSyntaxTreeNode;


#endif // RULES_H_INCLUDED
