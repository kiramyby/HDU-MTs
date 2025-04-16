#include <stdio.h>
#include <stdlib.h>

// Structure to represent a state in our simulation
typedef struct {
    int n;          // Number of disks to move
    char source;    // Source pole
    char auxiliary; // Auxiliary pole
    char destination; // Destination pole
} State;

// Stack structure for our non-recursive implementation
typedef struct {
    State* array;
    int top;
    int capacity;
} Stack;

// Function to create a stack of given capacity
Stack* createStack(int capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (State*)malloc(stack->capacity * sizeof(State));
    return stack;
}

// Function to check if the stack is full
int isFull(Stack* stack) {
    return stack->top == stack->capacity - 1;
}

// Function to check if the stack is empty
int isEmpty(Stack* stack) {
    return stack->top == -1;
}

// Function to add an item to the stack
void push(Stack* stack, State item) {
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
}

// Function to remove an item from the stack
State pop(Stack* stack) {
    State emptyState = {0, ' ', ' ', ' '};
    if (isEmpty(stack))
        return emptyState;
    return stack->array[stack->top--];
}

// Non-recursive function to solve Tower of Hanoi puzzle
void towerOfHanoi(int n) {
    // Create a stack with sufficient capacity
    Stack* stack = createStack(2 * n);
    
    // Initial state: Move n disks from A to C using B
    State initial = {n, 'A', 'B', 'C'};
    push(stack, initial);
    
    while (!isEmpty(stack)) {
        State current = pop(stack);
        
        if (current.n == 1) {
            // If only one disk, move it directly
            printf("%c->%c\n", current.source, current.destination);
        } else {
            // For more than one disk, break it down:
            
            // Step 3: Move n-1 disks from auxiliary to destination
            State step3 = {current.n - 1, current.auxiliary, current.source, current.destination};
            push(stack, step3);
            
            // Step 2: Move the largest disk from source to destination
            State step2 = {1, current.source, current.auxiliary, current.destination};
            push(stack, step2);
            
            // Step 1: Move n-1 disks from source to auxiliary
            State step1 = {current.n - 1, current.source, current.destination, current.auxiliary};
            push(stack, step1);
        }
    }
    
    // Free allocated memory
    free(stack->array);
    free(stack);
}

int main() {
    int n;
    scanf("%d", &n);
    
    if (n <= 6) { // As per problem constraint
        towerOfHanoi(n);
    }
    
    return 0;
}
