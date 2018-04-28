#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>

typedef struct StackNode
{
    int data;
    struct StackNode* next;
    struct StackNode* prev;

} StackNode;

int gettop(StackNode** top);
void push(StackNode** top, int data);
int pop(StackNode** top);
void printStack(StackNode** top);


#endif // STACK_H_INCLUDED
