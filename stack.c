#include "stack.h"

int gettop(StackNode** top)
{
    if (*top != NULL)
        return (*top)->data;

    return -1;
}

void push(StackNode** top, int data)
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

int pop(StackNode** top)
{
    if (*top != NULL)
    {
        int temp = (*top)->data;
        *top = (*top)->prev;
        return temp;
    }
    return -1;
}

void printStack(StackNode** top)
{
    StackNode* aux;
    for (aux = *top; aux != NULL; aux = aux->prev)
        printf("%d\n", aux->data);

    printf("\n");
}
