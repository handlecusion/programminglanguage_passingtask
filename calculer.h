#ifndef CALCULER_H
#define CALCULER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char* items;
    int top;
    int maxSize;
} Stack;

char	*evaluate(char *c);
void 	initialize(Stack* stack, int maxSize);
int 	isEmpty(Stack* stack);
int 	isFull(Stack* stack);
void 	push(Stack* stack, char item);
char 	pop(Stack* stack);
char 	peek(Stack* stack);
int 	getPriority(char op);
void 	infixToPostfix(char* infix, char* postfix);
int 	evaluatePostfix(char* postfix);
char	*itoa(int n);

#endif
