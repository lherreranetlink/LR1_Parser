#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include <string.h>
#include "grammar.h"
#include "lex.h"
#include "token.h"
#include "stack.h"
#include "rules.h"

#define CONTINUE_PARSE 0
#define PARSE_FINISH 1

void parse(FILE* fd);
void fileNotFoundError();
void initializeStack();
int getNextSymbolType(FILE* fd);
void pushTokenIntoStack(Token* token, int goToState);
void buildSyntaxTreeNode(int rule, int rows, int columns, ParserTableCell parserTable[rows][columns]);
void syntaxError();

void printSyntaxTree();
void printDefinitions(Definitions* definitions);
void printDefinition(GenericSyntaxTreeNode* definition);
void printVarDefinition(DefVar* defVar);
void printFunctionDefinition(DefFunc* defFunc);
void printVarList(VarList* varList);
void printParams(Params* params);
void printfFunctionBlok(FuncBlock* funcBlock);
void printParamList(ParamList* paramList);
void printLocalDefinitions(LocalVarsDef* localVarsDef);
void printLocalVarsAndStatements(GenericSyntaxTreeNode* localDefinition);
void printSingleStatement(GenericSyntaxTreeNode* singleStm);
void printAssignmentStatement(Statement* assignmentStm);
void printIfStatement(Statement_1* ifStm);
void printWhileStatement(Statement_2* whileStm);
void printReturnStatement(Statement_3* returnStm);
void printFunctionCallStatement(Statement_4* funcCallStm);
void printExpression(GenericSyntaxTreeNode* expression);
void printTerm(GenericSyntaxTreeNode* term);
void printFunctionCallTerm(FuncCall* funcCall);
void printBlockStatement(GenericSyntaxTreeNode* blockStm);
void printMultipleStatements(Statements* multipleStm);
void printBlock(Block* block);
void printMultipleStatements(Statements* multipleStm);
void printElsePart(ElsePart* elsePart);
void printFunctionCallArgs(Args* args);
void printFunctionCallArgList(ArgList* argList);

#endif // PARSER_H_INCLUDED
