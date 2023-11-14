#include <stdio.h>
#include <stdlib.h>

// 스택을 위한 구조체 정의
typedef struct {
    char* items;
    int top;
    int maxSize;
} Stack;

// 스택 초기화
void initialize(Stack* stack, int maxSize) {
    stack->items = (char*)malloc(sizeof(char) * maxSize);
    stack->top = -1;
    stack->maxSize = maxSize;
}

// 스택이 비어있는지 확인
int isEmpty(Stack* stack) {
    return stack->top == -1;
}

// 스택이 가득 찼는지 확인
int isFull(Stack* stack) {
    return stack->top == stack->maxSize - 1;
}

// 스택에 아이템 추가
void push(Stack* stack, char item) {
    if (isFull(stack)) {
        fprintf(stderr, "Error: Stack is full.\n");
        exit(EXIT_FAILURE);
    }
    stack->items[++(stack->top)] = item;
}

// 스택에서 아이템 제거 및 반환
char pop(Stack* stack) {
    if (isEmpty(stack)) {
        fprintf(stderr, "Error: Stack is empty.\n");
        exit(EXIT_FAILURE);
    }
    return stack->items[(stack->top)--];
}

// 스택에서 top 아이템 반환
char peek(Stack* stack) {
    if (isEmpty(stack)) {
        fprintf(stderr, "Error: Stack is empty.\n");
        exit(EXIT_FAILURE);
    }
    return stack->items[stack->top];
}

// 연산자 우선순위 반환
int getPriority(char op) {
    if (op == '+' || op == '-')
        return 1;
    else if (op == '*' || op == '/')
        return 2;
    else
        return 0;  // 괄호는 가장 높은 우선순위
}

// 중위 표기법을 후위 표기법으로 변환
void infixToPostfix(char* infix, char* postfix) {
    Stack stack;
    initialize(&stack, 100);
    int i, j = 0;

    for (i = 0; infix[i] != '\0'; i++) {
        if (infix[i] >= '0' && infix[i] <= '9') {
            postfix[j++] = infix[i];
        } else if (infix[i] == '(') {
            push(&stack, infix[i]);
        } else if (infix[i] == ')') {
            while (!isEmpty(&stack) && peek(&stack) != '(') {
                postfix[j++] = pop(&stack);
            }
            pop(&stack); // '(' 제거
        } else {
            while (!isEmpty(&stack) && getPriority(peek(&stack)) >= getPriority(infix[i])) {
                postfix[j++] = pop(&stack);
            }
            push(&stack, infix[i]);
        }
    }

    // 스택에 남아있는 연산자들을 후위 표기법으로 이동
    while (!isEmpty(&stack)) {
        postfix[j++] = pop(&stack);
    }

    postfix[j] = '\0'; // 문자열 종료
}

// 후위 표기법을 계산
int evaluatePostfix(char* postfix) {
    Stack stack;
    initialize(&stack, 100);
    int i, operand;

    for (i = 0; postfix[i] != '\0'; i++) {
        if (postfix[i] >= '0' && postfix[i] <= '9') {
            operand = postfix[i] - '0';
            push(&stack, operand);
        } else {
            int operand2 = pop(&stack);
            int operand1 = pop(&stack);
            switch (postfix[i]) {
                case '+':
                    push(&stack, operand1 + operand2);
                    break;
                case '-':
                    push(&stack, operand1 - operand2);
                    break;
                case '*':
                    push(&stack, operand1 * operand2);
                    break;
                case '/':
                    push(&stack, operand1 / operand2);
                    break;
            }
        }
    }

    return pop(&stack);
}
