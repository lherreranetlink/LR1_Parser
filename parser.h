#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include "grammar.h"
#include "lex.h"
#include "token.h"
#include "stack.h"

void parse(FILE* fd);
int getNextSymbolType(FILE* fd);
void appendSyntaxTreeNode();

#endif // PARSER_H_INCLUDED
