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

class Node {
public: 
   string name;
   int x;
   int y;
};

class Graph {
private:
    int** matrix;
    int N;
    int E;
    Node** nodes;

    int getIndex(string name) {
        for (int i = 0; i < N; i++) {
            if (nodes[i].name == name) {
                return i;
            }
        }
        return -1;
    };
public:
    /* Description: Constructor for the Graph class that initializes an N x N
    -               adjacency matrix filled with zeros.
    - Parameters: n - number of nodes in the graph
    - Return Value: None
    - Complexity: O(n^2)
     */
    Graph(int n) {
        N = n;
        E = 0;
        matrix = new int*[N];
        for (int i = 0; i < N; i++) {
            matrix[i] = new int[N]();
        }
    }

    /* Description: Destructor for the Graph class that deallocates each row
    -              and the main pointer array of the adjacency matrix.
    - Parameters: None
    - Return Value: None
    - Complexity: O(n)
     */
    ~Graph() {
        for (int i = 0; i < N; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    void setNode(int index, string name, int x, int y) {
        nodes[index].name = name;
        nodes[index].x = x;
        nodes[index].y = y;
    }

    /* Description: Adds an edge between nodes a and b with a given weight
    -               and directionality into the adjacency matrix.
    - Parameters: a - source node
    -             b - destination node
    -             weight - weight of the edge
    -             dir - true if it is directional, false if it's not
    - Return Value: None (void function)
    - Complexity: O(1)
     */
    void addEdge(int a, int b, int weight, bool dir) {
        matrix[a][b] = weight;
        if (dir == false) {
            matrix[b][a] = weight;
        }
        E++;
    }

    /* Description: Prints the entire N x N adjacency matrix with row and column
    -              indices.
    - Parameters: None
    - Return Value: None (void function)
    - Complexity: O(n^2)
     */
    void printMatrix() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << "[" << i << "," << j << "]" << matrix[i][j] << "\t\t";
            }
            cout << endl;
        }
        cout << endl;
    }

    /* Description: Determines if the graph is connected by using
    -              BFS traversal from node 0 and checking if all
    -              N nodes are visited.
    - Parameters: None
    - Return Value: bool (true if all nodes are reachable, if not false)
    - Complexity: O(n^2)
     */
    bool isConnected() {
        if (N == 0) {
            return true;
        }
        bool* visited = new bool [N];
        for (int i = 0; i < N; i++) {
            visited[i] = false;
        }
        int count = 0;
        visited[0] = true;
        Queue q(N);
        q.enqueue(0);
        while (!q.isEmpty()) {
            int curr = q.dequeue();
            count++;
            for (int i = 0; i < N; i++) {
                if (matrix[curr][i] != 0 && visited[i] == false) {
                    visited[i] = true;
                    q.enqueue(i);
                }
            }
        }
        delete[] visited;
        if (count == N) {
            return true;
        } else {
            return false;
        }
    }

    /* Description: Performs a Depth-First Search (DFS) starting from
    -              a given node using a Stack and prints the traversal order.
    - Parameters: start - starting node index
    - Return Value: None (void function)
    - Complexity: O(n^2)
     */
    void DFS(int start) {
        bool* visited = new bool [N];
        for (int i = 0; i < N; i++) {
            visited[i] = false;
        }
        Stack s(N);
        s.push(start);
        while (!s.isEmpty()) {
            int curr = s.pop();
            if (visited[curr] == false) {
                visited[curr] = true;
                cout << curr << " ";
                for (int i = 0; i < N; i++) {
                    if (matrix[curr][i] != 0 && visited[i] == false) {
                        visited[curr] = true;
                        s.push(i);
                    }
                }
            }
        }
        cout << endl;
        delete[] visited;
    }

    /* Description: Performs a Breadth-First Search (BFS) starting from a given
    -              node using a Queue and prints the traversal order.
    - Parameters: start - starting node index
    - Return Value: None (void function)
    - Complexity: O(n^2)
     */
    void BFS(int start) {
        bool* visited = new bool [N];
        for (int i = 0; i < N; i++) {
            visited[i] = false;
        }
        visited[start] = true;
        Queue q(N);
        q.enqueue(start);
        while (!q.isEmpty()) {
            int curr = q.dequeue();
            cout << curr << " ";
            for (int i = 0; i < N; i++) {
                if (matrix[curr][i] != 0 && visited[i] == false) {
                    visited[i] = true;
                    q.enqueue(i);
                }
            }
        }
        cout << endl;
        delete[] visited;
    }

    /* Description: Calculates the shortest path weights and paths from a given start
    -              node to all other nodes using Dijkstra's algorithm and
    -              prints the results.
    - Parameters: start - source node index
    - Return Value: None (void function)
    - Complexity: O(n^2)
     */
    void dijkstra(int start) {
        const int inf = 1e9;
        int* distance = new int[N];
        int* processed = new int[N];
        int* from = new int[N];
        for (int i = 0; i < N; i++) {
            distance[i] = inf;
            processed[i] = 0;
            from[i] = 0;
        }
        distance[start] = 0;
        for (int i = 0; i < N; i++) {
            int u = 0;
            while (processed[u] == 1) {
                u++;
            }
            for (int j = 0; j < N; j++) {
                if (processed[j] == 0 && distance[j] < distance[u]) {
                    u = j;
                }
            }
            processed[u] = 1;
            for (int k = 0; k < N; k++) {
                if (matrix[u][k] != 0) {
                    int newDist = distance[u] + matrix[u][k];
                    if (newDist < distance[k]) {
                        distance[k] = newDist;
                        from[k] = u;
                    }
                }
            }
        }
        for (int i = 0; i < N; i++) {
            cout << start << " to " << i << " (cost = " << distance[i] << "): ";
            int curr = i;
            while (curr != start) {
                cout << curr << " <- ";
                curr = from[curr];
            }
            cout << start << endl;
        }
        delete[] distance;
        delete[] processed;
        delete[] from;
    }

    /* Description: Finds and prints the edges of the Minimum Spanning Tree (MST)
    -              using Prim's algorithm starting from node 0.
    - Parameters: None
    - Return Value: None (void function)
    - Complexity: O(n^2)
     */
    void primMST() {
        const int inf = 1e9;
        int* distance = new int[N];
        int* processed = new int[N];
        int* from = new int[N];
        for (int i = 0; i < N; i++) {
            distance[i] = inf;
            processed[i] = 0;
            from[i] = -1;
        }
        distance[0] = 0;
        for (int i = 0; i < N; i++) {
            int u = 0;
            while (processed[u] == 1) {
                u++;
            }
            for (int j = 0; j < N; j++) {
                if (processed[j] == 0 && distance[j] < distance[u]) {
                    u = j;
                }
            }
            processed[u] = 1;
            for (int k = 0; k < N; k++) {
                if (matrix[u][k] != 0 && processed[k] == 0) {
                    int newDist = matrix[u][k];
                    if (newDist < distance[k]) {
                        distance[k] = newDist;
                        from[k] = u;
                    }
                }
            }
        }
        for (int i = 1; i < N; i++) {
            cout << from[i] << " - " << i << " (weight = " << distance[i] << ")" << endl;
        }
        delete[] distance;
        delete[] processed;
        delete[] from;
    }

    /* Description: Finds and prints the edges of the Minimum Spanning Tree (MST)
    -              using Kruskal's algorithm by selecting the minimum
    -              weight edge connecting two separate parts of the graph.
    - Parameters: None
    - Return Value: None (void function)
    - Complexity: O(n^3)
     */
    void kruskalMST() {
        const int inf = 1e9;
        int* group = new int[N];
        for (int i = 0; i < N; i++) {
            group[i] = i;
        }
        int nSelected = 0;
        while (nSelected < N-1) {
            int weight = inf;
            int u = 0;
            int v = 0;
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    if (matrix[i][j] != 0 && group[i] != group[j] && matrix[i][j] < weight) {
                        weight = matrix[i][j];
                        u = i;
                        v = j;
                    }
                }
            }
            cout << u << " - " << v << " (weight = " << weight << ")" << endl;
            nSelected++;
            int oldGroup = group[v];
            for (int i = 0; i < N; i++) {
                if (group[i] == oldGroup) {
                    group[i] = group[u];
                }
            }
        }
        delete[] group;
    }
};



