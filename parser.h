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

#endif // PARSER_H_INCLUDED
