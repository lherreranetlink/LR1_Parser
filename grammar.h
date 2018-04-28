#ifndef GRAMMAR_H_INCLUDED
#define GRAMMAR_H_INCLUDED
#include <stdio.h>
#include <string.h>
#include "token.h"
#include "lex.h"
#include "fileutils.h"
#include "stringutils.h"

#define GO_TO_ERROR 0

typedef enum TransitionType {SHIFT, REDUCTION, GO_TO_STATE, ERROR} TransitionType;

typedef struct Rule
{
    int index;
    int ruleSize;
    char* name;
    struct Rule* next;
} Rule;

typedef struct ParserTableCell
{
    TransitionType transitionType;
    int goTo;
} ParserTableCell;

Rule* buildGrammar(FILE* fd);
int* getParserTableDimentions(FILE* fd);
void buildTransitionsTable(FILE* fd, int rows, int columns, ParserTableCell parserTable[rows][columns]);
void insertRule(Rule** header, Rule* nextRule);
Rule* getRuleByPosition(Rule** header, int position);
int getRuleSize(Rule** header, Rule* rule);
void insertInitialTransition(Rule** header);

void printRulesList(Rule** header);
void printParserTable(int rows, int columns, ParserTableCell table[rows][columns]);

#endif // GRAMMAR_H_INCLUDED
