#include "parser.h"

static Token* currentToken;
static StackNode* stackTop;
static Rule* rulesListHeader;

void parse(FILE* fd)
{
    FILE* grammarRulesFile;
    if ((grammarRulesFile = fopen("grammar", "r")) == NULL)
        fileNotFoundError();

    rulesListHeader = buildGrammar(grammarRulesFile);
    int* parserTableDimentions = getParserTableDimentions(grammarRulesFile);
    int rows = parserTableDimentions[0], columns = parserTableDimentions[1];

    ParserTableCell parserTable[rows][columns];
    buildTransitionsTable(grammarRulesFile, rows, columns, parserTable);

    insertInitialTransition(&rulesListHeader);

    int parseFinish = 0, currentTokenType;
    initializeStack();
    ParserTableCell currentState;
    while (!parseFinish)
    {
        currentState = parserTable[gettop(&stackTop)->stateToSee][currentTokenType = getNextSymbolType(fd)];
        switch (currentState.transitionType)
        {
        case SHIFT:
            pushTokenIntoStack(currentToken, currentState.goTo);
            break;
        case REDUCTION:
        {
            int i, ruleSize;
            ParserTableCell goToState;

            Rule* ruleToReduce = getRuleByPosition(&rulesListHeader, currentState.goTo);
            if (ruleToReduce->index != 0)
            {
                /*ruleSize = getRuleSize(&rulesListHeader, ruleToReduce);
                for (i = 0; i < ruleSize * 2; i++)
                    pop(&stackTop);

                ungetToken(currentToken);

                goToState = parserTable[gettop(&stackTop)][ruleToReduce->index];
                if (goToState.transitionType == ERROR)
                    syntaxError();

                push(&stackTop, ruleToReduce->index);
                push(&stackTop, goToState.goTo);*/

            }
            else
                parseFinish = 1;
        }
        break;
        case ERROR:
            syntaxError();
            break;
        case GO_TO_STATE:
            ;
        }
    }
}

void fileNotFoundError()
{
    fprintf(stderr, "File with grammar rules does not exists\n");
    exit(1);
}

void initializeStack()
{
    char tokenSymbol[] = "$";

    Token* eofToken = generateNewToken("$", EOF_SIGN);

    GenericSyntaxTreeNode* initialStackNode = (GenericSyntaxTreeNode*) malloc(sizeof(GenericSyntaxTreeNode));
    initialStackNode->stateToSee = 0;
    initialStackNode->attr.token = eofToken;

    push(&stackTop, initialStackNode);
}

int getNextSymbolType(FILE* fd)
{
    currentToken = getNextToken(fd);
    return currentToken->type;
}

void pushTokenIntoStack(Token* token, int goToState)
{
    GenericSyntaxTreeNode* stackNode = (GenericSyntaxTreeNode*) malloc(sizeof(GenericSyntaxTreeNode));
    stackNode->stateToSee = goToState;
    stackNode->attr.token = token;

    push(&stackTop, stackNode);
}

void syntaxError()
{
    fprintf(stderr, "Syntax Error: Unexpected token: %s\n", currentToken->symbol);
    exit(1);
}
