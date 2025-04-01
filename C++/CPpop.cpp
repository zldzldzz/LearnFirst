#include <stdio.h>
#include <iostream>

//#define STACK_SIZE 10

int STACK_SIZE = 10;
int* stack;
int top = -1;               // top 초기화
int capacity = 1;

// 스택이 포화 상태인지 확인하는 연산
bool isFull() {
	if (top % STACK_SIZE == 9) return true;
	else return false;
}

int isEmpty() {
	if (top == -1) return 1;
	else return 0;
}

// 스택의 top에 원소를 삽입하는 연산
void push(int item) {
	int* temp;

	if (isFull()) {
		printf("Full STACK\n");
		temp = new int[STACK_SIZE * capacity];
		for (int i = 0; i < (top + 1); i++) temp[i] = stack[i];
		delete [] stack;
		stack = new int[STACK_SIZE * (capacity + 1)];
		for (int i = 0; i < (top + 1); i++) stack[i] = temp[i];
		stack[++top] = item;  // top을 증가시킨 후 현재 top에 원소 삽입
		capacity++;
		delete [] temp;

	}
	else stack[++top] = item;  // top을 증가시킨 후 현재 top에 원소 삽입
}

int pop() {
	int* temp;

	if (isEmpty()) {	// 스택이 공백 상태인 경우
		printf("\n\n Stack is Empty!!\n");
		return 0;
	}
	else
	{
		// 동적 배열로 구현
		int i, re;

		if (top % STACK_SIZE == 0)
		{
			printf("CHANGE STACK\n");
			re = stack[top];
			temp = new int[STACK_SIZE * (capacity-1)];
			for (i = 0; i < top; i++) temp[i] = stack[i];
			delete [] stack;
			stack = new int[STACK_SIZE * (capacity-1)];
			for (i = 0; i < top; i++) stack[i] = temp[i];
			top--;
			capacity--;
			delete [] temp;


			return re;

		}
		else
			return stack[top--];	      // 현재 top의 원소를 삭제한 후 top 감소
	}
}


// 스택의 원소를 출력하는 연산
void printStack() {
	int i;
	printf("STACK SIZE [%d]\n", (STACK_SIZE * capacity));
	printf("\n STACK [ ");
	for (i = 0; i <= top; i++) printf("%d ", stack[i]);
	printf("]\n");
}

int main() {
    int i;
    stack = new int[STACK_SIZE * capacity];

    for (i = 0; i < 34; i++) push(i + 1);

    printStack();
	
    for (i = 0; i < 34; i++) printf("POP Data [%d]\n", pop());

    delete[] stack;

    getchar(); // 입력 대기
    return 0;  // 정상 종료 반환값 추가
}