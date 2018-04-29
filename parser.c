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
        int transitionType = currentState.transitionType;
        switch (transitionType)
        {
        case SHIFT:
            pushTokenIntoStack(currentToken, currentState.goTo);
            break;
        case REDUCTION:
            buildSyntaxTreeNode(currentState.goTo, rows, columns, parserTable);
            ungetToken(currentToken);
            break;
        case ERROR:
            syntaxError();
            break;
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
    Token* eofToken = generateNewToken("$", EOF_SIGN);
    GenericSyntaxTreeNode* node = (GenericSyntaxTreeNode*) malloc(sizeof(GenericSyntaxTreeNode));
    node->ruleType = SIMPLE_TOKEN;
    node->stateToSee = 0;
    node->attr.simpleToken = eofToken;
}

int getNextSymbolType(FILE* fd)
{
    currentToken = getNextToken(fd);
    return currentToken->type;
}

void pushTokenIntoStack(Token* token, int goToState)
{
    GenericSyntaxTreeNode* node = (GenericSyntaxTreeNode*) malloc(sizeof(GenericSyntaxTreeNode));
    node->ruleType = SIMPLE_TOKEN;
    node->stateToSee = goToState;
    node->attr.simpleToken = token;
}

void buildSyntaxTreeNode(int rule, int rows, int columns, ParserTableCell parserTable[rows][columns])
{
    GenericSyntaxTreeNode* node = (GenericSyntaxTreeNode*) malloc(sizeof(GenericSyntaxTreeNode));

    switch(rule)
    {
    case PROGRAM:
    {
        Program* program = (Program*) malloc(sizeof(Program));
        program->definitions = pop(&stackTop);
        node->ruleType = PROGRAM;
        node->attr.program = program;
    }
    break;
    case DEFINITIONS_1:
    {
        Definitions* definitions = (Definitions*) malloc(sizeof(Definitions));
        definitions->moreDefinitions = pop(&stackTop);
        definitions->definition = pop(&stackTop);
        node->ruleType = DEFINITIONS_1;
        node->attr.definitions = definitions;
    }
    break;
    case DEFINITION:
    {
        Definition* definition = (Definition*) malloc(sizeof(Definition));
        definition->defVar = pop(&stackTop);
        node->ruleType = DEFINITION;
        node->attr.definition = definition;
    }
    break;
    case DEFINITION_1:
    {

    }
    break;
    case DEF_VAR:
        pop(&stackTop);
        DefVar* defVar = (DefVar*) malloc(sizeof(DefVar));
        defVar->varList = pop(&stackTop);
        defVar->identifier = pop(&stackTop);
        defVar->dataType = pop(&stackTop);
        node->ruleType = DEF_VAR;
        node->attr.defVar = defVar;
        break;
    case VAR_LIST_1:
    {

    }
    break;
    case DEF_FUNC:
    {

    }
    break;
    case PARAMETERS_1:
    {

    }
    break;
    case PARAMS_LIST:
    {

    }
    break;
    case PARAMS_LIST_1:
    {

    }
    break;
    case FUNC_BLOCK:
    {

    }
    break;
    case LOCAL_VARS_DEF_1:
    {

    }
    break;
    case LOCAL_VAR_DEF:
    {

    }
    break;
    case LOCAL_VAR_DEF_1:
    {

    }
    break;
    case STATEMENTS_1:
    {

    }
    break;
    case STATEMENT:
    {

    }
    break;
    case STATEMENT_1:
    {

    }
    break;
    case STATEMENT_2:
    {

    }
    break;
    case STATEMENT_3:
    {

    }
    break;
    case STATEMENT_4:
    {

    }
    break;
    case ELSE_PART_1:
    {

    }
    break;
    case BLOCK:
    {

    }
    break;
    case RETURN_VALUE_1:
    {

    }
    break;
    case ARGS_1:
    {

    }
    break;
    case ARG_LIST_1:
    {

    }
    break;
    case TERM:
    {

    }
    break;
    case TERM_1:
    {

    }
    break;
    case TERM_2:
    {

    }
    break;
    case TERM_3:
    {

    }
    break;
    case TERM_4:
    {

    }
    break;
    case FUNC_CALL:
    {

    }
    break;
    case BLOCK_STM:
    {

    }
    break;
    case BLOCK_STM_1:
    {

    }
    break;
    case EXPRESSION:
    {

    }
    break;
    case EXPRESSION_1:
    {

    }
    break;
    case EXPRESSION_2:
    {

    }
    break;
    case EXPRESSION_3:
    {

    }
    break;
    case EXPRESSION_4:
    {

    }
    break;
    case EXPRESSION_5:
    {

    }
    break;
    case EXPRESSION_6:
    {

    }
    break;
    case EXPRESSION_7:
    {

    }
    break;
    case EXPRESSION_8:
    {

    }
    break;
    case EXPRESSION_9:
    {

    }
    break;
    case VAR_LIST:
    case DEFINITIONS:
    case PARAMETERS:
    case LOCAL_VARS_DEF:
    case STATEMENTS:
    case ELSE_PART:
    case RETURN_VALUE:
    case ARGS:
    case ARG_LIST:
        node->ruleType = EPSILON_RULE;
        break;
    }

    ParserTableCell goToState;
    Rule* ruleToReduce = getRuleByPosition(&rulesListHeader, rule);
    goToState = parserTable[gettop(&stackTop)->stateToSee][ruleToReduce->index];

    if (goToState.transitionType == ERROR)
        syntaxError();

    node->stateToSee = goToState.goTo;
    push(&stackTop, node);
}

void syntaxError()
{
    fprintf(stderr, "Syntax Error: Unexpected token: %s\n", currentToken->symbol);
    exit(1);
}
