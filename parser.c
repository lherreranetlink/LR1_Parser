#include "parser.h"

static Token* currentToken;
static StackNode* stackTop;
static Rule* rulesListHeader;
static GenericSyntaxTreeNode* syntaxTreeHeader;

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

    int parseFinish = CONTINUE_PARSE, currentTokenType;
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
        {
            buildSyntaxTreeNode(currentState.goTo, rows, columns, parserTable);
            ungetToken(currentToken);

            if (gettop(&stackTop)->stateToSee == PROGRAM && currentToken->type == EOF_SIGN)
                parseFinish = PARSE_FINISH;
        }
        break;
        case ERROR:
            syntaxError();
            break;
        }
    }
    syntaxTreeHeader = pop(&stackTop);
    printSyntaxTree();
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

    push(&stackTop, node);
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

    push(&stackTop, node);
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
        Definition_1* definition_1 = (Definition_1*) malloc(sizeof(Definition_1));
        definition_1->defFunc = pop(&stackTop);
        node->ruleType = DEFINITION_1;
        node->attr.definition_1 = definition_1;
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
        VarList* varList = (VarList*) malloc(sizeof(VarList));
        varList->nextVar = pop(&stackTop);
        varList->identifier = pop(&stackTop);
        pop(&stackTop);

        node->ruleType = VAR_LIST_1;
        node->attr.varList = varList;
    }
    break;
    case DEF_FUNC:
    {
        DefFunc* defFunc = (DefFunc*) malloc(sizeof(DefFunc));
        defFunc->blockFunc = pop(&stackTop);
        pop(&stackTop);
        defFunc->params = pop(&stackTop);
        pop(&stackTop);
        defFunc->identifier = pop(&stackTop);
        defFunc->dataType = pop(&stackTop);

        node->ruleType = DEF_FUNC;
        node->attr.defFunc = defFunc;
    }
    break;
    case PARAMETERS_1:
    {
        Params* params = (Params*) malloc(sizeof(Params));
        params->paramList = pop(&stackTop);
        params->identifier = pop(&stackTop);
        params->dataType = pop(&stackTop);

        node->ruleType = PARAMETERS_1;
        node->attr.params = params;
    }
    break;
    case PARAMS_LIST_1:
    {
        ParamList* paramList = (ParamList*) malloc(sizeof(ParamList));
        paramList->paramList = pop(&stackTop);
        paramList->identifier = pop(&stackTop);
        paramList->dataType = pop(&stackTop);
        pop(&stackTop);

        node->ruleType = PARAMS_LIST_1;
        node->attr.paramList = paramList;
    }
    break;
    case FUNC_BLOCK:
    {
        FuncBlock* funcBlock = (FuncBlock*) malloc(sizeof(FuncBlock));
        pop(&stackTop);
        funcBlock->localVarsDef = pop(&stackTop);
        pop(&stackTop);

        node->ruleType = FUNC_BLOCK;
        node->attr.funcBlock = funcBlock;
    }
    break;
    case LOCAL_VARS_DEF_1:
    {
        LocalVarsDef* localVarsDef = malloc(sizeof(LocalVarsDef));
        localVarsDef->localVarsDef = pop(&stackTop);
        localVarsDef->localVarDef = pop(&stackTop);

        node->ruleType = LOCAL_VARS_DEF_1;
        node->attr.localVarsDef = localVarsDef;
    }
    break;
    case LOCAL_VAR_DEF:
    {
        LocalVarDef* localVarDef = malloc(sizeof(LocalVarDef));
        localVarDef->defVar = pop(&stackTop);

        node->ruleType = LOCAL_VAR_DEF;
        node->attr.localVarDef = localVarDef;
    }
    break;
    case LOCAL_VAR_DEF_1:
    {
        LocalVarDef_1* localVarDef_1 = malloc(sizeof(LocalVarDef_1));
        localVarDef_1->statement = pop(&stackTop);

        node->ruleType = LOCAL_VAR_DEF_1;
        node->attr.localVarDef_1 = localVarDef_1;
    }
    break;
    case STATEMENTS_1:
    {
        Statements* statements = malloc(sizeof(Statement));
        statements->statements = pop(&stackTop);
        statements->statement = pop(&stackTop);

        node->ruleType = STATEMENTS_1;
        node->attr.statements = statements;
    }
    break;
    case STATEMENT:
    {
        Statement* statement = malloc(sizeof(Statement));
        pop(&stackTop);
        statement->expression = pop(&stackTop);
        statement->equalsSign = pop(&stackTop);
        statement->identifier = pop(&stackTop);

        node->ruleType = STATEMENT;
        node->attr.statement = statement;
    }
    break;
    case STATEMENT_1:
    {
        Statement_1* statement_1 = malloc(sizeof(Statement_1));
        statement_1->partElse = pop(&stackTop);
        statement_1->partIf = pop(&stackTop);
        pop(&stackTop);
        statement_1->expression = pop(&stackTop);
        pop(&stackTop);
        pop(&stackTop);

        node->ruleType = STATEMENT_1;
        node->attr.statement_1 = statement_1;
    }
    break;
    case STATEMENT_2:
    {
        Statement_2* statement_2 = malloc(sizeof(Statement_2));
        statement_2->block = pop(&stackTop);
        pop(&stackTop);
        statement_2->expression = pop(&stackTop);
        pop(&stackTop);
        pop(&stackTop);

        node->ruleType = STATEMENT_2;
        node->attr.statement_2 = statement_2;
    }
    break;
    case STATEMENT_3:
    {
        Statement_3* statement_3 = malloc(sizeof(Statement_3));
        pop(&stackTop);
        statement_3->returnValue = pop(&stackTop);
        pop(&stackTop);

        node->ruleType = STATEMENT_3;
        node->attr.statement_3 = statement_3;
    }
    break;
    case STATEMENT_4:
    {
        Statement_4* statement_4 = malloc(sizeof(Statement_4));
        pop(&stackTop);
        statement_4->funcCall = pop(&stackTop);

        node->ruleType = STATEMENT_4;
        node->attr.statement_4 = statement_4;
    }
    break;
    case ELSE_PART_1:
    {
        ElsePart* elsePart = malloc(sizeof(ElsePart));
        elsePart->blockStm = pop(&stackTop);
        pop(&stackTop);

        node->ruleType = ELSE_PART_1;
        node->attr.elsePart = elsePart;
    }
    break;
    case BLOCK:
    {
        Block* block = malloc(sizeof(Block));
        pop(&stackTop);
        block->statements = pop(&stackTop);
        pop(&stackTop);

        node->ruleType = BLOCK;
        node->attr.block = block;
    }
    break;
    case RETURN_VALUE_1:
    {
        ReturnValue* returnValue = malloc(sizeof(ReturnValue));
        returnValue->expression = pop(&stackTop);

        node->ruleType = RETURN_VALUE_1;
        node->attr.returnValue = returnValue;
    }
    break;
    case ARGS_1:
    {
        Args* args = malloc(sizeof(Args));
        args->argList = pop(&stackTop);
        args->expression = pop(&stackTop);

        node->ruleType = ARGS_1;
        node->attr.args = args;
    }
    break;
    case ARG_LIST_1:
    {
        ArgList* argList = malloc(sizeof(ArgList));
        argList->argList = pop(&stackTop);
        argList->expression = pop(&stackTop);
        pop(&stackTop);

        node->ruleType = ARG_LIST_1;
        node->attr.argList = argList;
    }
    break;
    case TERM:
    {
        Term* term = malloc(sizeof (Term));
        term->funcCall = pop(&stackTop);

        node->ruleType = TERM;
        node->attr.term = term;
    }
    break;
    case TERM_1:
    {
        Term_1* term_1 = malloc(sizeof(Term_1));
        term_1->identifier = pop(&stackTop);

        node->ruleType = TERM_1;
        node->attr.term_1 = term_1;
    }
    break;
    case TERM_2:
    {
        Term_2* term_2 = malloc(sizeof(Term_2));
        term_2->integer = pop(&stackTop);

        node->ruleType = TERM_2;
        node->attr.term_2 = term_2;
    }
    break;
    case TERM_3:
    {
        Term_3* term_3 = malloc(sizeof(Term_3));
        term_3->realNumber = pop(&stackTop);

        node->ruleType = TERM_3;
        node->attr.term_3 = term_3;
    }
    break;
    case TERM_4:
    {
        Term_4* term_4 = malloc(sizeof(Term_4));
        term_4->constChar = pop(&stackTop);

        node->ruleType = TERM_4;
        node->attr.term_4 = term_4;
    }
    break;
    case FUNC_CALL:
    {
        FuncCall* funcCall = malloc(sizeof(FuncCall));
        pop(&stackTop);
        funcCall->args = pop(&stackTop);
        pop(&stackTop);
        funcCall->identifier = pop(&stackTop);

        node->ruleType = FUNC_CALL;
        node->attr.funcCall = funcCall;
    }
    break;
    case BLOCK_STM:
    {
        BlockStm* blockStm = malloc(sizeof(BlockStm));
        blockStm->statement = pop(&stackTop);

        node->ruleType = BLOCK_STM;
        node->attr.blockStm = blockStm;
    }
    break;
    case BLOCK_STM_1:
    {
        BlockStm_1* blockStm_1 = malloc(sizeof(BlockStm_1));
        blockStm_1->block = pop(&stackTop);

        node->ruleType = BLOCK_STM_1;
        node->attr.blockStm_1 = blockStm_1;
    }
    break;
    case EXPRESSION:
    {
        Expression* expression = malloc(sizeof(Expression));
        pop(&stackTop);
        expression->expression = pop(&stackTop);
        pop(&stackTop);

        node->ruleType = EXPRESSION;
        node->attr.expression = expression;
    }
    break;
    case EXPRESSION_1:
    {
        Expression_1* expression_1 = malloc(sizeof(Expression_1));
        expression_1->expression = pop(&stackTop);
        expression_1->additionOperator = pop(&stackTop);

        node->ruleType = EXPRESSION_1;
        node->attr.expression_1 = expression_1;
    }
    break;
    case EXPRESSION_2:
    {
        Expression_2* expression_2 = malloc(sizeof(Expression_2));
        expression_2->expression = pop(&stackTop);
        expression_2->multOperator = pop(&stackTop);

        node->ruleType = EXPRESSION_2;
        node->attr.expression_2 = expression_2;
    }
    break;
    case EXPRESSION_3:
    {
        Expression_3* expression_3 = malloc(sizeof(Expression_3));
        expression_3->expressionLeft = pop(&stackTop);
        expression_3->multOperator = pop(&stackTop);
        expression_3->expressionRight = pop(&stackTop);

        node->ruleType = EXPRESSION_3;
        node->attr.expression_3 = expression_3;
    }
    break;
    case EXPRESSION_4:
    {
        Expression_4* expression_4 = malloc(sizeof(Expression_4));
        expression_4->expressionLeft = pop(&stackTop);
        expression_4->additionOperator = pop(&stackTop);
        expression_4->expressionRight = pop(&stackTop);

        node->ruleType = EXPRESSION_4;
        node->attr.expression_4 = expression_4;
    }
    break;
    case EXPRESSION_5:
    {
        Expression_5* expression_5 = malloc(sizeof(Expression_5));
        expression_5->expressionLeft = pop(&stackTop);
        expression_5->relationalOperator = pop(&stackTop);
        expression_5->expressionRight = pop(&stackTop);

        node->ruleType = EXPRESSION_5;
        node->attr.expression_5 = expression_5;
    }
    break;
    case EXPRESSION_6:
    {
        Expression_6* expression_6 = malloc(sizeof(Expression_6));
        expression_6->expressionLeft = pop(&stackTop);
        expression_6->equalsComparisonOperator = pop(&stackTop);
        expression_6->expressionRight = pop(&stackTop);

        node->ruleType = EXPRESSION_6;
        node->attr.expression_6 = expression_6;
    }
    break;
    case EXPRESSION_7:
    {
        Expression_7* expression_7 = malloc(sizeof(Expression_7));
        expression_7->expressionLeft = pop(&stackTop);
        expression_7->andOperator = pop(&stackTop);
        expression_7->expressionRight = pop(&stackTop);

        node->ruleType = EXPRESSION_7;
        node->attr.expression_7 = expression_7;
    }
    break;
    case EXPRESSION_8:
    {
        Expression_8* expression_8 = malloc(sizeof(Expression_8));
        expression_8->expressionLeft = pop(&stackTop);
        expression_8->orOperator = pop(&stackTop);
        expression_8->expressionRight = pop(&stackTop);

        node->ruleType = EXPRESSION_8;
        node->attr.expression_8 = expression_8;
    }
    break;
    case EXPRESSION_9:
    {
        Expression_9* expression_9 = malloc(sizeof(Expression_9));
        expression_9->term = pop(&stackTop);

        node->ruleType = EXPRESSION_9;
        node->attr.expression_9 = expression_9;
    }
    break;
    case VAR_LIST:
    case DEFINITIONS:
    case PARAMETERS:
    case PARAMS_LIST:
    case LOCAL_VARS_DEF:
    case STATEMENTS:
    case ELSE_PART:
    case RETURN_VALUE:
    case ARGS:
    case ARG_LIST:
        node->ruleType = EPSILON_RULE;
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

void printSyntaxTree()
{
    Program* program = syntaxTreeHeader->attr.program;
    fprintf(stdout, "<Program>\n");

    if (program->definitions->ruleType != EPSILON_RULE)
        printDefinitions(program->definitions->attr.definitions);

    fprintf(stdout, "</Program>\n");
}

void printDefinitions(Definitions* definitions)
{
    fprintf(stdout, "<Definitions>\n");

    printDefinition(definitions->definition);
    if (definitions->moreDefinitions->ruleType != EPSILON_RULE)
        printDefinitions(definitions->moreDefinitions->attr.definitions);

    fprintf(stdout, "</Definitions>\n");
}

void printDefinition(GenericSyntaxTreeNode* definition)
{
    int defType = definition->ruleType;
    switch (defType)
    {
    case DEFINITION:
        printVarDefinition(definition->attr.definition->defVar->attr.defVar);
        break;
    case DEFINITION_1:
        printFunctionDefinition(definition->attr.definition_1->defFunc->attr.defFunc);
    }
}

void printVarDefinition(DefVar* defVar)
{
    fprintf(stdout, "<DefVar>\n");
    fprintf(stdout, "Data Type: %s\n", defVar->dataType->attr.simpleToken->symbol);
    fprintf(stdout, "Identifier: %s\n", defVar->identifier->attr.simpleToken->symbol);

    if (defVar->varList->ruleType != EPSILON_RULE)
        printVarList(defVar->varList->attr.varList);

    fprintf(stdout, "</DefVar>\n");
}

void printFunctionDefinition(DefFunc* defFunc)
{
    fprintf(stdout, "<DefFunc>\n");
    fprintf(stdout, "Data Type:\n");
    fprintf(stdout, "Identifier:\n");
    printParams();
    printfFunctionBlok();
    fprintf(stdout, "</DefFunc>\n");
}

void printVarList(VarList* varList)
{
    fprintf(stdout, "<VarList>\n");
    fprintf(stdout, "Identifier: %s\n", varList->identifier->attr.simpleToken->symbol);

    if (varList->nextVar->ruleType != EPSILON_RULE)
        printVarList(varList->nextVar->attr.varList);

    fprintf(stdout, "</VarList>\n");
}

void printParams()
{

}

void printfFunctionBlok()
{

}
