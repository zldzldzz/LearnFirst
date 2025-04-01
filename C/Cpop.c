#include <stdio.h>
#include <stdlib.h>
#define STACK_SIZE 10

int *stack;
int top = -1; // top 초기화
int capacity = 1;

int isEmpty()
{
	if (top == -1)
		return 1;
	else
		return 0;
}

int pop()
{
	if (isEmpty())
	{ // 스택이 공백 상태인 경우
		printf("\n\n Stack is Empty!!\n");
		return 0;
	}
	else
	{
		// 동적 배열로 구현
		int i, re, *temp;

		if (top % STACK_SIZE == 0)
		{
			printf("CHANGE STACK\n");
			re = stack[top];
			temp = (int *)malloc(STACK_SIZE * (capacity - 1) * sizeof(int));
			for (i = 0; i < top; i++)
				temp[i] = stack[i];
			free(stack);
			stack = (int *)malloc(STACK_SIZE * (capacity - 1) * sizeof(int));
			for (i = 0; i < top; i++)
				stack[i] = temp[i];
			free(temp);
			top--;
			capacity--;

			return re;
		}
		else
			return stack[top--]; // 현재 top의 원소를 삭제한 후 top 감소
	}
}

// 스택이 포화 상태인지 확인하는 연산
int isFull()
{
	if (top % STACK_SIZE == 9)
		return 1;
	else
		return 0;
}

// 스택의 top에 원소를 삽입하는 연산
void push(int item)
{
	int *temp;

	if (isFull())
	{
		printf("Full STACK\n");
		temp = (int *)malloc((STACK_SIZE * capacity) * sizeof(int));
		for (int i = 0; i < (top + 1); i++)
			temp[i] = stack[i];
		free(stack);
		stack = (int *)malloc((STACK_SIZE * (capacity + 1)) * sizeof(int));
		for (int i = 0; i < (top + 1); i++)
			stack[i] = temp[i];
		stack[++top] = item; // top을 증가시킨 후 현재 top에 원소 삽입
		capacity++;
		free(temp);
	}
	else
		stack[++top] = item; // top을 증가시킨 후 현재 top에 원소 삽입
}

// 스택의 원소를 출력하는 연산
void printStack()
{
	int i;
	printf("STACK SIZE [%d]\n", (STACK_SIZE * capacity));
	printf("\n STACK [ ");
	for (i = 0; i <= top; i++)
		printf("%d ", stack[i]);
	printf("]\n");
}

void main(void)
{
	int i;
	stack = (int *)malloc(STACK_SIZE * sizeof(int));

	for (i = 0; i < 24; i++)
		push(i + 1);

	printStack();

	for (i = 0; i < 7; i++)
		printf("POP Data [%d]\n", pop());

	printStack();

	free(stack);

	getchar();
}