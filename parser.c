#include "parser.h"

static Token* currentToken;

void parse(FILE* fd)
{
    FILE* grammarRulesFile;
    if ((grammarRulesFile = fopen("grammar", "r")) == NULL)
    {
        fprintf(stderr, "File with grammar rules does not exists\n");
        exit(1);
    }

    Rule* rulesListHeader = buildGrammar(grammarRulesFile);
    StackNode* stackTop = NULL;

    int* parserTableDimentions = getParserTableDimentions(grammarRulesFile);
    int rows = parserTableDimentions[0], columns = parserTableDimentions[1];

    ParserTableCell parserTable[rows][columns];
    buildTransitionsTable(grammarRulesFile, rows, columns, parserTable);

    insertInitialTransition(&rulesListHeader);

    int parseFinish = 0, currentTokenType;
    push(&stackTop, EOF_SIGN);
    push(&stackTop, 0);
    ParserTableCell currentState;
    while (!parseFinish)
    {
        currentState = parserTable[gettop(&stackTop)][currentTokenType = getNextSymbolType(fd)];
        switch (currentState.transitionType)
        {
        case SHIFT:
            push(&stackTop, currentTokenType);
            push(&stackTop, currentState.goTo);
            break;
        case REDUCTION:
        {
            int i, ruleSize;
            ParserTableCell goToState;

            Rule* ruleToReduce = getRuleByPosition(&rulesListHeader, currentState.goTo);
            if (ruleToReduce->index != 0)
            {
                ruleSize = getRuleSize(&rulesListHeader, ruleToReduce);
                for (i = 0; i < ruleSize * 2; i++)
                    pop(&stackTop);

                ungetToken(currentToken);

                goToState = parserTable[gettop(&stackTop)][ruleToReduce->index];
                if (goToState.transitionType == ERROR)
                {
                    fprintf(stderr, "Syntax Error: Unexpected token: %s\n", currentToken->symbol);
                    exit(1);
                }
                push(&stackTop, ruleToReduce->index);
                push(&stackTop, goToState.goTo);

            }
            else
                parseFinish = 1;
        }
        break;
        case ERROR:
        {
            fprintf(stderr, "Syntax Error: Unexpected token %s\n", currentToken->symbol);
            exit(1);
        }
        break;
        case GO_TO_STATE:
            ;
        }
    }
}

int getNextSymbolType(FILE* fd)
{
    currentToken = getNextToken(fd);
    if (currentToken->type == ERROR_STATE)
    {
        fprintf(stderr, "Invalid token: %s\n", currentToken->symbol);
        exit(1);
    }
    return currentToken->type;
}
