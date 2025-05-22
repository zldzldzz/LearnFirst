#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define QUEUE_SIZE 10

int capacity = 1;
typedef struct { 
	int front;
	int rear;
	int* data;
} QueueType;

void init_queue(QueueType* q)
{
	q->front = q->rear = 0;
	q->data = new int[QUEUE_SIZE * capacity];
}

void delete_queue(QueueType* q)
{
	delete[] q->data;
}

int is_empty(QueueType* q)
{
	return (q->front == q->rear);
}

int is_full(QueueType *q)
{
    return ((q->rear + 1) % (QUEUE_SIZE * capacity) == q->front);
}

void queue_print(QueueType* q)
{
	printf("QUEUE SIZE [%d]\n", (QUEUE_SIZE * capacity));
	printf("QUEUE(front=%d rear=%d) = ", q->front, q->rear);
	if (!is_empty(q)) {
		int i = q->front;
		do {
			i = (i + 1) % (QUEUE_SIZE * capacity);
			printf("%d | ", q->data[i]);
			if (i == q->rear)
				break;
		} while (i != q->front);
	}
	printf("\n");
}

void enqueue(QueueType* q, int item)
{
	int* temp;
	if (is_full(q)) {
		printf("Full QUEUE\n");
		temp = (int*)malloc((QUEUE_SIZE * capacity) * sizeof(int));
		if (q->rear > q->front)
			for (int i = 0; i < (q->rear + 1); i++) temp[i] = q->data[i];
		else {
			for (int i = q->front; i < (QUEUE_SIZE * capacity); i++) temp[i - q->front] = q->data[i];
			for (int i = 0; i < (q->rear + 1); i++) temp[QUEUE_SIZE * capacity - (q->rear + 1) + i] = q->data[i];
			q->front = 0;
			q->rear = QUEUE_SIZE * capacity - 1;
		}
		free(q->data);
		q->data = (int*)malloc((QUEUE_SIZE * (capacity + 1)) * sizeof(int));
		for (int i = 0; i < (q->rear + 1); i++) q->data[i] = temp[i];
		//q->data[++(q->rear)] = item;  // q->rear         Ų         q->rear            
		capacity++;
		free(temp);
	}
	q->rear = (q->rear + 1) % (QUEUE_SIZE * capacity);
	q->data[q->rear] = item;
}

int dequeue(QueueType* q)
{
	if (is_empty(q)) {
		printf("QUEUE is Empty!!\n");
		return 0;
	}

	if (abs(q->rear - q->front) % QUEUE_SIZE == 0) {
		printf("CHANGE DEQUEUE\n");
		int item = q->data[q->front + 1];
		int* temp = (int*)malloc((QUEUE_SIZE * (capacity - 1)) * sizeof(int));

		if (q->rear > q->front)
			for (int i = q->front + 1; i < q->rear + 1; i++) temp[i - (q->front + 1)] = q->data[i];
		else {
			for (int i = q->front + 1; i < QUEUE_SIZE * capacity; i++) temp[i - (q->front + 1)] = q->data[i];
			for (int i = 0; i < (q->rear + 1); i++) temp[QUEUE_SIZE * (capacity - 1) - (q->rear + 1) + i] = q->data[i];
		}

		free(q->data);
		q->data = (int*)malloc((QUEUE_SIZE * (capacity - 1)) * sizeof(int));

		for (int i = 0; i < QUEUE_SIZE * (capacity - 1); i++) q->data[i] = temp[i];

		q->front = 0;
		q->rear = QUEUE_SIZE * (capacity - 1) - 1;
		capacity--;

		free(temp);
		return item;
	}
	else {
		q->front = (q->front + 1) % (QUEUE_SIZE * capacity);
		return q->data[q->front];
	}
}

void enqueueAt(QueueType *q, int index, int item)
{
    if (is_full(q))
    {
        printf("Full QUEUE, resizing...\n");
        // enqueue와 동일한 크기 확장 로직
        int *temp = (int *)malloc((QUEUE_SIZE * capacity) * sizeof(int));
        if (q->rear >= q->front)
        {
            for (int i = q->front; i <= q->rear; i++)
                temp[i - q->front] = q->data[i];
            q->rear = q->rear - q->front;
            q->front = 0;
        }
        else
        {
            for (int i = q->front; i < QUEUE_SIZE * capacity; i++)
                temp[i - q->front] = q->data[i];
            for (int i = 0; i <= q->rear; i++)
                temp[QUEUE_SIZE * capacity - q->front + i] = q->data[i];
            q->rear = QUEUE_SIZE * capacity - q->front + q->rear;
            q->front = 0;
        }
        free(q->data);
        q->data = (int *)malloc((QUEUE_SIZE * (capacity + 1)) * sizeof(int));
        for (int i = 0; i <= q->rear; i++)
            q->data[i] = temp[i];
        capacity++;
        free(temp);
    }

    // index 유효성 검사
    if (index < 0 || index > (q->rear - q->front + (q->rear < q->front ? QUEUE_SIZE * capacity : 0)))
    {
        printf("Invalid index!\n");
        return;
    }

    // rear부터 index까지 요소를 한 칸씩 뒤로 이동
    int actual_index = (q->front + index) % (QUEUE_SIZE * capacity);
    for (int i = q->rear; i >= actual_index; i--)
        q->data[(i + 1) % (QUEUE_SIZE * capacity)] = q->data[i % (QUEUE_SIZE * capacity)];

    // index 위치에 item 삽입
    q->data[actual_index] = item;
    q->rear = (q->rear + 1) % (QUEUE_SIZE * capacity);
}

// =====     ť  ڵ     ======

int main(void)
{
	QueueType q;
	init_queue(&q);

	for (int i = 1; i <= 15; i++) {
		enqueue(&q, i * 1);
		queue_print(&q);
	}

	for (int i = 0; i < 5; i++) {
		printf("Dequeue: %d\n", dequeue(&q));
		queue_print(&q);
	}

	enqueueAt(&q, 2, 999);  // 앞에서 2번째 자리에 999 삽입
	queue_print(&q);
	enqueueAt(&q, 5, 999);  // 앞에서 5번째 자리에 999 삽입
	queue_print(&q);

	delete_queue(&q);

	return 0;
}
