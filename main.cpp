#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Stack {
private:
    int* data;
    int top;
    int maxSize;
public:
    /* Description: Constructor for the Stack class that dynamically allocates
    -               an array with the capacity it's given.
    - Parameters: capacity - maximum capacity of the stack (initially)
    - Return Value: None
    - Complexity: O(1)
     */
    Stack(int capacity) {
        if (capacity > 0) {
            maxSize = capacity;
        } else {
            maxSize = 1;
        }
        data = new int[maxSize];
        top = -1;
    }

    /* Description: Destructor for the Stack class that frees the dynamically
    -               allocated array.
    - Parameters: None
    - Return Value: None
    - Complexity: O(1)
     */
    ~Stack() {
        delete[] data;
    }

    /* Description: Pushes a new integer onto the top of the stack, resizing the
    -               array if the stack is full.
    - Parameters: value - value to push onto the stack
    - Return Value: None (void function)
    - Complexity: O(n) when resizing (but it's O(1) if it doesn't need to resize)
     */
    void push(int value) {
        if (isFull()) {
            int newMaxSize = maxSize + 1;
            int* newData = new int[newMaxSize];
            for (int i = 0; i <= top; i++) {
                newData[i] = data[i];
            }
            delete data;
            data = newData;
            maxSize = newMaxSize;
        }
        data[++top] = value;
    }

    /* Description: Removes and returns the integer at the top of the stack.
    - Parameters: None
    - Return Value: int (element popped)
    - Complexity: O(1)
     */
    int pop() {
        return data[top--];
    }

    /* Description: Returns the integer at the top of the stack without removing it.
    - Parameters: None
    - Return Value: int (element at the top of the stack)
    - Complexity: O(1)
     */
    int getTop() {
        return data[top];
    }

    /* Description: Checks if the stack has no elements.
    - Parameters: None
    - Return Value: bool (true if empty, if not false)
    - Complexity: O(1)
     */
    bool isEmpty() {
        return top == -1;
    }

    /* Description: Checks if the stack has reached its capacity.
    - Parameters: None
    - Return Value: bool (true if full, if not false)
    - Complexity: O(1)
     */
    bool isFull() {
        return top == maxSize - 1;
    }

    /* Description: Returns the current number of elements in the stack.
    - Parameters: None
    - Return Value: int (number of elements in the stack)
    - Complexity: O(1)
     */
    int getSize() {
        return top + 1;
    }
};

class Queue {
private:
    int* data;
    int front;
    int end;
    int size;
    int maxSize;
public:
    /* Description: Constructor for the Queue class that dynamically allocates
    -               an array with the capacity it's given.
    - Parameters: capacity - maximum capacity of the queue (initially)
    - Return Value: None
    - Complexity: O(1)
     */
    Queue(int capacity) {
        if (capacity > 0) {
            maxSize = capacity;
        } else {
            maxSize = 1;
        }
        data = new int[maxSize];
        front = -1;
        end = -1;
        size = 0;
    }

    /* Description: Destructor for the Queue class that frees the dynamically
    -              allocated array.
    - Parameters: None
    - Return Value: None
    - Complexity: O(1)
     */
    ~Queue() {
        delete[] data;
    }

    /* Description: Inserts a new integer at the back of the queue,
    -              and it resizes the array if the queue is full.
    - Parameters: value - value to enqueue
    - Return Value: None (void function)
    - Complexity: O(n) when resizing (but it's O(1) if it doesn't need to resize)
     */
    void enqueue(int value) {
        if (isFull()) {
            int newMaxSize = maxSize + 1;
            int* newData = new int[newMaxSize];
            for (int i = 0; i < size; i++) {
                newData[i] = data[(front + i) % maxSize];
            }
            delete data;
            data = newData;
            maxSize = newMaxSize;
        }
        if (isEmpty()) {
            front = 0;
            end = 0;
        } else {
            end = (end + 1) % maxSize;
        }
        data[end] = value;
        size++;
    }

    /* Description: Removes and returns the integer at the front of the queue.
    - Parameters: None
    - Return Value: int (element removed from the front of the queue)
    - Complexity: O(1)
     */
    int dequeue() {
        int value = data[front];
        size--;
        if (isEmpty()) {
            front = -1;
            end = -1;
        } else {
            front = (front + 1) % maxSize;
        }
        return value;
    }

    /* Description: Returns the integer at the front of the queue without removing it.
    - Parameters: None
    - Return Value: int (element at the front of the queue)
    - Complexity: O(1)
     */
    int getFront() {
        return data[front];
    }

    /* Description: Checks if the queue has no elements.
    - Parameters: None
    - Return Value: bool (true if empty, if not false)
    - Complexity: O(1)
     */
    bool isEmpty() {
        return size == 0;
    }

    /* Description: Checks if the queue has reached its capacity.
    - Parameters: None
    - Return Value: bool (true if full, if not false)
    - Complexity: O(1)
     */
    bool isFull() {
        return size == maxSize;
    }

    /* Description: Returns the current number of elements in the queue.
    - Parameters: None
    - Return Value: int (number of elements in the queue)
    - Complexity: O(1)
     */
    int getSize() {
        return size;
    }
};
