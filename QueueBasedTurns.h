#include <stdio.h>
#include <stdlib.h>

// Define a structure for the queue
struct Queue {
    int front, rear;
    unsigned capacity;
    char** items;
};

// Create a new queue
struct Queue* createQueue(unsigned capacity) {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->rear = -1;
    queue->items = (char**)malloc(capacity * sizeof(char*));
    return queue;
}

// Check if the queue is empty
int isEmpty(struct Queue* queue) {
    return queue->front == -1;
}

// Enqueue an item into the queue
void enqueue(struct Queue* queue, char* item) {
    if (queue->rear == queue->capacity - 1) {
        printf("Queue is full. Cannot enqueue.\n");
        return;
    }
    queue->items[++queue->rear] = item;
    if (queue->front == -1) {
        queue->front = 0;
    }
}

// Dequeue an item from the queue
char* dequeue(struct Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty. Cannot dequeue.\n");
        return NULL;
    }
    char* item = queue->items[queue->front];
    if (queue->front == queue->rear) {
        queue->front = queue->rear = -1;
    } else {
        queue->front++;
    }
    return item;
}

// Example usage
int main() {
    struct Queue* combatQueue = createQueue(10);

    // Enqueue characters (players and enemies)
    enqueue(combatQueue, "Player 1");
    enqueue(combatQueue, "Enemy A");
    enqueue(combatQueue, "Player 2");

    // Process turns
    while (!isEmpty(combatQueue)) {
        char* currentCharacter = dequeue(combatQueue);
        printf("%s's turn!\n", currentCharacter);
        // Execute the character's action here (e.g., attack, defend).
    }

    // Clean up memory
    free(combatQueue->items);
    free(combatQueue);

    return 0;
}

