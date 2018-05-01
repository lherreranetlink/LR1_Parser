#include "stack.h"

GenericSyntaxTreeNode* gettop(StackNode** top)
{
    if (*top != NULL)
        return (*top)->data;

    return NULL;
}

void push(StackNode** top, GenericSyntaxTreeNode* data)
{
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;

    if(*top == NULL)
        *top = newNode;
    else
    {
        (*top)->next = newNode;
        newNode->prev = *top;
        *top = newNode;
    }
}

GenericSyntaxTreeNode* pop(StackNode** top)
{
    if (*top != NULL)
    {
        GenericSyntaxTreeNode* temp = (*top)->data;
        *top = (*top)->prev;
        return temp;
    }
    return NULL;
}

void printStack(StackNode** top)
{
    /*StackNode* aux;
    for (aux = *top; aux != NULL; aux = aux->prev)
        printf("%d\n", aux->data);

    printf("\n");*/
}
