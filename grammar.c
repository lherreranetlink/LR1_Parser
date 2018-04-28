#include "grammar.h"

Rule* buildGrammar(FILE* fd)
{
    Rule* listHeader = NULL;
    char* line;

    for (line = getNextLine(fd); strcmp(line, "%") != 0; line = getNextLine(fd))
    {
        char** attrs = split(line, '\t');
        Rule* newRule = (Rule*) malloc(sizeof(Rule));
        newRule->index = atoi(attrs[0]);
        newRule->ruleSize = atoi(attrs[1]);
        newRule->name = (char*) malloc(strlen(attrs[2]));
        strcpy(newRule->name, attrs[2]);
        insertRule(&listHeader, newRule);
        free(line);
    }

    return listHeader;
}

int* getParserTableDimentions(FILE* fd)
{
    int* dimentions;
    char* line;
    int rowsCount = 0, columnsCount = 0;
    for (line = getNextLine(fd); strcmp(line, "%") != 0; line = getNextLine(fd))
    {
        char** attrs = split(line, '\t');
        rowsCount = atoi(attrs[0]);
        columnsCount = atoi(attrs[1]);
        free(attrs);
    }
    free(line);
    dimentions = (int*) malloc(sizeof(int) * 2);
    dimentions[0] = rowsCount;
    dimentions[1] = columnsCount;
    return dimentions;
}

void buildTransitionsTable(FILE* fd, int rows, int columns, ParserTableCell parserTable[rows][columns])
{
    int i, j;
    for (i = 0; i < rows; i++)
    {
        char* line = getNextLine(fd);
        char** attrs = split(line, '\t');
        for (j = 0; j < columns; j++)
        {
            ParserTableCell newComponent;
            if (strcmp(attrs[j], "-0") == 0)
            {
                newComponent.transitionType = REDUCTION;
                newComponent.goTo = 0;
            }
            else
            {
                int cellValue = atoi(attrs[j]);
                if (cellValue > 0)
                {
                    newComponent.transitionType = (j > EOF_SIGN)? GO_TO_STATE:SHIFT;
                    newComponent.goTo = cellValue;
                }
                else if (cellValue < 0)
                {
                    newComponent.transitionType = REDUCTION;
                    newComponent.goTo = (cellValue * -1);
                }
                else
                {
                    newComponent.transitionType = ERROR;
                    newComponent.goTo = GO_TO_ERROR;
                }
            }
            parserTable[i][j] = newComponent;
        }
        free(attrs);
        free(line);
    }
}

void insertRule(Rule** header, Rule* nextRule)
{
    if (*header == NULL)
        *header = nextRule;
    else
    {
        Rule* aux;
        for (aux = *header; aux->next != NULL; aux = aux->next);
        aux->next = nextRule;
    }
}

Rule* getRuleByPosition(Rule** header, int position)
{
    if (*header != NULL)
    {
        Rule* aux;
        for (aux = *header; position-- > 0; aux = aux->next);
        return aux;
    }
    return NULL;
}

int getRuleSize(Rule** header, Rule* rule)
{
    if (*header != NULL)
    {
        Rule* aux;
        for (aux = *header; aux != NULL; aux = aux->next)
        {
            if (aux == rule)
                return aux->ruleSize;
        }
    }
    return 0;
}

void insertInitialTransition(Rule** header)
{
    if (*header != NULL)
    {
        char name[] = "Initial";
        Rule* initialRule = (Rule*) malloc(sizeof(Rule));
        initialRule->index = 0;
        initialRule->ruleSize = 1;
        initialRule->name = (char*) malloc(strlen(name));
        strcpy(initialRule->name, name);
        initialRule->next = *header;
        *header = initialRule;
    }
}

void printRulesList(Rule** header)
{
    if (*header != NULL)
    {
        Rule* aux;
        for (aux = *header; aux != NULL; aux = aux->next)
        {
            printf("Index: %d\n", aux->index);
            printf("Size: %d\n", aux->ruleSize);
            printf("Name: %s\n\n", aux->name);
        }
    }
}

void printParserTable(int rows, int columns, ParserTableCell table[rows][columns])
{
    int i, j;
    for (i = 0; i < rows; i++)
        for(j = 0; j < columns; j++)
        {
            printf("Transition Type: %d\n", table[i][j].transitionType);
            printf("Go To: %d\n\n", table[i][j].goTo);
        }

}
