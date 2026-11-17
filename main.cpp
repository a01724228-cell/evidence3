#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// Value returned by pop, getTop, dequeue and getFront when the structure is
// empty. It is never a valid node index (indices are 0 or greater), so it
// cannot be confused with real data. The caller should check isEmpty() first.
const int EMPTY_VALUE = -1;

// Value stored in the adjacency matrix when there is NO connection.
// A connection with cost 0 is valid, so 0 cannot mean "no connection".
const int NO_EDGE = -1;

// Value used as "infinite" distance. It is never added to another cost.
const int INFINITE_COST = 1000000000;

// Maximum cost accepted for one connection (avoids overflow when adding).
const int MAX_COST = 1000000;

const string DEFAULT_FILE_NAME = "test.txt";

const int STATE_COUNT = 4;

class Stack {
private:
    int* data = nullptr;
    int top = -1;
    int maxSize = 1;
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

    // Copies are disabled: a default copy would share the same array and
    // both destructors would free it twice.
    Stack(const Stack& other) = delete;
    Stack& operator=(const Stack& other) = delete;

    /* Description: Destructor for the Stack class that frees the dynamically
    -               allocated array.
    - Parameters: None
    - Return Value: None
    - Complexity: O(1)
     */
    ~Stack() {
        delete[] data;
    }

    /* Description: Pushes a new integer onto the top of the stack, doubling
    -               the capacity of the array if the stack is full.
    - Parameters: value - value to push onto the stack
    - Return Value: None (void function)
    - Complexity: O(n) when resizing (but it's O(1) if it doesn't need to resize)
     */
    void push(int value) {
        if (isFull()) {
            int newMaxSize = maxSize * 2;
            int* newData = new int[newMaxSize];
            for (int i = 0; i <= top; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
            maxSize = newMaxSize;
        }
        top++;
        data[top] = value;
    }

    /* Description: Removes and returns the integer at the top of the stack.
    -               If the stack is empty it prints an error and returns
    -               EMPTY_VALUE without modifying the stack.
    - Parameters: None
    - Return Value: int (element popped, or EMPTY_VALUE if the stack is empty)
    - Complexity: O(1)
     */
    int pop() {
        int value = EMPTY_VALUE;
        if (isEmpty()) {
            cout << "  [Error] pop: la pila esta vacia." << endl;
        } else {
            value = data[top];
            top--;
        }
        return value;
    }

    /* Description: Returns the integer at the top of the stack without removing it.
    - Parameters: None
    - Return Value: int (element at the top, or EMPTY_VALUE if the stack is empty)
    - Complexity: O(1)
     */
    int getTop() {
        int value = EMPTY_VALUE;
        if (isEmpty()) {
            cout << "  [Error] getTop: la pila esta vacia." << endl;
        } else {
            value = data[top];
        }
        return value;
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

    /* Description: Returns the current capacity of the internal array.
    - Parameters: None
    - Return Value: int (capacity of the stack)
    - Complexity: O(1)
     */
    int getCapacity() {
        return maxSize;
    }
};

class Queue {
private:
    int* data = nullptr;
    int front = -1;
    int end = -1;
    int size = 0;
    int maxSize = 1;
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

    // Copies are disabled for the same reason as in Stack.
    Queue(const Queue& other) = delete;
    Queue& operator=(const Queue& other) = delete;

    /* Description: Destructor for the Queue class that frees the dynamically
    -               allocated array.
    - Parameters: None
    - Return Value: None
    - Complexity: O(1)
     */
    ~Queue() {
        delete[] data;
    }

    /* Description: Inserts a new integer at the back of the queue. If the
    -               queue is full, it doubles the capacity and copies the
    -               elements in order (from front to end) to positions
    -               0 .. size - 1, so front and end are updated.
    - Parameters: value - value to enqueue
    - Return Value: None (void function)
    - Complexity: O(n) when resizing (but it's O(1) if it doesn't need to resize)
     */
    void enqueue(int value) {
        if (isFull()) {
            int newMaxSize = maxSize * 2;
            int* newData = new int[newMaxSize];
            for (int i = 0; i < size; i++) {
                newData[i] = data[(front + i) % maxSize];
            }
            delete[] data;
            data = newData;
            maxSize = newMaxSize;
            front = 0;
            end = size - 1;
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
