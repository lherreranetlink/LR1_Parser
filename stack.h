#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "rules.h"

typedef struct StackNode
{
    GenericSyntaxTreeNode* data;
    struct StackNode* next;
    struct StackNode* prev;

} StackNode;

GenericSyntaxTreeNode* gettop(StackNode** top);
void push(StackNode** top, GenericSyntaxTreeNode* data);
GenericSyntaxTreeNode* pop(StackNode** top);
void printStack(StackNode** top);


#endif // STACK_H_INCLUDED
