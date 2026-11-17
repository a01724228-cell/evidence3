/* CAMPUS CONNECT!
- Description: Plans the network of connections between the buildings of a
-              university. It reads the buildings and the possible connections
-              from a text file, builds the installation plan of minimum total
-              cost (Kruskal), simulates the failure of a connection, restores
-              the network, adds a new building and compares the configurations.
-              It also allows route queries over the installed network.
- Authors: [NOMBRE] - [MATRICULA]
-          [NOMBRE] - [MATRICULA]
- Date: [FECHA]
 */
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

    /* Description: Removes and returns the integer at the front of the queue.
    -               If the queue is empty it prints an error and returns
    -               EMPTY_VALUE without modifying the queue.
    - Parameters: None
    - Return Value: int (element removed, or EMPTY_VALUE if the queue is empty)
    - Complexity: O(1)
     */
    int dequeue() {
        int value = EMPTY_VALUE;
        if (isEmpty()) {
            cout << "  [Error] dequeue: la cola esta vacia." << endl;
        } else {
            value = data[front];
            size--;
            if (isEmpty()) {
                front = -1;
                end = -1;
            } else {
                front = (front + 1) % maxSize;
            }
        }
        return value;
    }

    /* Description: Returns the integer at the front of the queue without removing it.
    - Parameters: None
    - Return Value: int (element at the front, or EMPTY_VALUE if the queue is empty)
    - Complexity: O(1)
     */
    int getFront() {
        int value = EMPTY_VALUE;
        if (isEmpty()) {
            cout << "  [Error] getFront: la cola esta vacia." << endl;
        } else {
            value = data[front];
        }
        return value;
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

    /* Description: Returns the current capacity of the internal array.
    - Parameters: None
    - Return Value: int (capacity of the queue)
    - Complexity: O(1)
     */
    int getCapacity() {
        return maxSize;
    }

    /* Description: Returns the array position of the front (used in tests to
    -               show that the queue moves circularly).
    - Parameters: None
    - Return Value: int (index of the front, -1 if empty)
    - Complexity: O(1)
     */
    int getFrontPosition() {
        return front;
    }

    /* Description: Returns the array position of the end (used in tests to
    -               show that the queue moves circularly).
    - Parameters: None
    - Return Value: int (index of the end, -1 if empty)
    - Complexity: O(1)
     */
    int getEndPosition() {
        return end;
    }
};

// Represents one building of the campus.
class Node {
public:
    string name = "";
    int x = 0;
    int y = 0;
};

// Represents one bidirectional connection between two node indices.
// It is always stored with from < to, so A-B and B-A are the same connection.
class Edge {
public:
    int from = -1;
    int to = -1;
    int cost = 0;
};

// Connection read from the file, still written with building names.
class NamedEdge {
public:
    string nameA = "";
    string nameB = "";
    int cost = 0;
};

// All the data read from the input file.
class InputData {
public:
    vector<Node> initialNodes;
    vector<NamedEdge> initialEdges;
    string failedNameA = "";
    string failedNameB = "";
    Node newNode;
    vector<NamedEdge> newEdges;
};

// Independent copy of one installed network (one stage of the simulation).
// It only contains vectors and simple values, so copying it with "=" creates
// a complete and independent copy.
class Configuration {
public:
    string title = "";
    bool exists = false;
    bool isComplete = false;
    int totalCost = 0;
    vector<Node> nodes;
    vector<Edge> edges;
};

/* Description: Creates an Edge, ordering its ends so that from < to.
- Parameters: a - index of one end
-             b - index of the other end
-             cost - installation cost of the connection
- Return Value: Edge (connection created)
- Complexity: O(1)
 */
Edge makeEdge(int a, int b, int cost) {
    Edge edge;
    if (a < b) {
        edge.from = a;
        edge.to = b;
    } else {
        edge.from = b;
        edge.to = a;
    }
    edge.cost = cost;
    return edge;
}

/* Description: Converts a boolean value into the text "SI" or "NO".
- Parameters: value - boolean value
- Return Value: string ("SI" if value is true, "NO" if it is false)
- Complexity: O(1)
 */
string yesNoText(bool value) {
    string text = "NO";
    if (value) {
        text = "SI";
    }
    return text;
}

/* Description: Builds the path from the start node to the end node using the
-              predecessor array of Dijkstra. The number of steps is limited
-              to the number of nodes, so it can never loop forever.
- Parameters: from - predecessor of each node (-1 if it has none)
-             endNode - last node of the path
- Return Value: vector<int> (node indices from start to endNode)
- Complexity: O(n)
 */
vector<int> buildPath(vector<int>& from, int endNode) {
    vector<int> path;
    int curr = endNode;
    int steps = 0;
    int maxSteps = from.size();
    while (curr != -1 && steps < maxSteps) {
        path.push_back(curr);
        curr = from[curr];
        steps++;
    }
    reverse(path.begin(), path.end());
    return path;
}

class Graph {
private:
    int** matrix = nullptr;
    int nodeCount = 0;
    int edgeCount = 0;
    Node* nodes = nullptr;

    /* Description: Checks if an index belongs to an existing node.
    - Parameters: index - index to check
    - Return Value: bool (true if 0 <= index < nodeCount, if not false)
    - Complexity: O(1)
     */
    bool isValidIndex(int index) {
        return index >= 0 && index < nodeCount;
    }

public:
    /* Description: Constructor for the Graph class that initializes an n x n
    -               adjacency matrix filled with NO_EDGE and an array of n
    -               nodes with empty names.
    - Parameters: n - number of nodes in the graph (0 is allowed)
    - Return Value: None
    - Complexity: O(n^2)
     */
    Graph(int n) {
        if (n > 0) {
            nodeCount = n;
        } else {
            nodeCount = 0;
        }
        edgeCount = 0;
        matrix = new int*[nodeCount];
        for (int i = 0; i < nodeCount; i++) {
            matrix[i] = new int[nodeCount];
            for (int j = 0; j < nodeCount; j++) {
                matrix[i][j] = NO_EDGE;
            }
        }
        nodes = new Node[nodeCount];
    }

    // Copies are disabled: the matrix and the nodes are dynamic arrays and a
    // default copy would share them. Configurations are saved with the
    // Configuration class instead.
    Graph(const Graph& other) = delete;
    Graph& operator=(const Graph& other) = delete;

    /* Description: Destructor for the Graph class that deallocates each row,
    -               the main pointer array of the adjacency matrix and the
    -               array of nodes.
    - Parameters: None
    - Return Value: None
    - Complexity: O(n)
     */
    ~Graph() {
        for (int i = 0; i < nodeCount; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
        delete[] nodes;
    }

    /* Description: Searches a node by its name.
    - Parameters: name - name of the building
    - Return Value: int (index of the node, or -1 if it does not exist)
    - Complexity: O(n)
     */
    int getIndex(string name) {
        int index = -1;
        int i = 0;
        while (i < nodeCount && index == -1) {
            if (nodes[i].name == name) {
                index = i;
            }
            i++;
        }
        return index;
    }

    /* Description: Stores the name and coordinates of an existing node.
    - Parameters: index - index of the node
    -             name - name of the building
    -             x - x coordinate
    -             y - y coordinate
    - Return Value: bool (true if the index was valid, if not false)
    - Complexity: O(1)
     */
    bool setNode(int index, string name, int x, int y) {
        bool valid = isValidIndex(index);
        if (valid) {
            nodes[index].name = name;
            nodes[index].x = x;
            nodes[index].y = y;
        }
        return valid;
    }

    /* Description: Adds a new node at the end. It creates a bigger matrix and
    -               a bigger node array, copies the previous data (so the
    -               previous connections are kept) and frees the old arrays.
    - Parameters: name - name of the new building
    -             x - x coordinate
    -             y - y coordinate
    - Return Value: bool (true if added, false if the name already exists or is empty)
    - Complexity: O(n^2)
     */
    bool addNode(string name, int x, int y) {
        bool canAdd = name != "" && getIndex(name) == -1;
        if (canAdd) {
            int newCount = nodeCount + 1;
            int** newMatrix = new int*[newCount];
            for (int i = 0; i < newCount; i++) {
                newMatrix[i] = new int[newCount];
                for (int j = 0; j < newCount; j++) {
                    if (i < nodeCount && j < nodeCount) {
                        newMatrix[i][j] = matrix[i][j];
                    } else {
                        newMatrix[i][j] = NO_EDGE;
                    }
                }
            }
            Node* newNodes = new Node[newCount];
            for (int i = 0; i < nodeCount; i++) {
                newNodes[i] = nodes[i];
            }
            newNodes[nodeCount].name = name;
            newNodes[nodeCount].x = x;
            newNodes[nodeCount].y = y;
            for (int i = 0; i < nodeCount; i++) {
                delete[] matrix[i];
            }
            delete[] matrix;
            delete[] nodes;
            matrix = newMatrix;
            nodes = newNodes;
            nodeCount = newCount;
        }
        return canAdd;
    }

    /* Description: Returns a copy of a node.
    - Parameters: index - index of the node
    - Return Value: Node (copy of the node, or an empty Node if the index is invalid)
    - Complexity: O(1)
     */
    Node getNode(int index) {
        Node node;
        if (isValidIndex(index)) {
            node = nodes[index];
        }
        return node;
    }

    /* Description: Returns the number of nodes.
    - Parameters: None
    - Return Value: int (number of nodes)
    - Complexity: O(1)
     */
    int getNodeCount() {
        return nodeCount;
    }

    /* Description: Returns the number of bidirectional connections.
    - Parameters: None
    - Return Value: int (number of connections, each one counted once)
    - Complexity: O(1)
     */
    int getEdgeCount() {
        return edgeCount;
    }

    /* Description: Checks if there is a connection between two nodes.
    - Parameters: a - index of one node
    -             b - index of the other node
    - Return Value: bool (true if the connection exists, if not false)
    - Complexity: O(1)
     */
    bool hasEdge(int a, int b) {
        return isValidIndex(a) && isValidIndex(b) && matrix[a][b] != NO_EDGE;
    }

    /* Description: Returns the cost of the connection between two nodes.
    - Parameters: a - index of one node
    -             b - index of the other node
    - Return Value: int (cost, or NO_EDGE if there is no connection)
    - Complexity: O(1)
     */
    int getCost(int a, int b) {
        int cost = NO_EDGE;
        if (hasEdge(a, b)) {
            cost = matrix[a][b];
        }
        return cost;
    }

    /* Description: Adds an edge between nodes a and b with a given weight
    -               and directionality into the adjacency matrix. It rejects
    -               invalid indices, loops, repeated edges and costs outside
    -               0 .. MAX_COST. This project always uses dir = false.
    - Parameters: a - source node
    -             b - destination node
    -             weight - weight (installation cost) of the edge
    -             dir - true if it is directional, false if it's not
    - Return Value: bool (true if the edge was added, if not false)
    - Complexity: O(1)
     */
    bool addEdge(int a, int b, int weight, bool dir) {
        bool canAdd = isValidIndex(a) && isValidIndex(b) && a != b;
        canAdd = canAdd && weight >= 0 && weight <= MAX_COST;
        canAdd = canAdd && matrix[a][b] == NO_EDGE;
        if (canAdd) {
            matrix[a][b] = weight;
            if (dir == false) {
                matrix[b][a] = weight;
            }
            edgeCount++;
        }
        return canAdd;
    }

    /* Description: Removes the bidirectional connection between a and b.
    - Parameters: a - index of one node
    -             b - index of the other node
    - Return Value: bool (true if it existed and was removed, if not false)
    - Complexity: O(1)
     */
    bool removeEdge(int a, int b) {
        bool exists = hasEdge(a, b);
        if (exists) {
            matrix[a][b] = NO_EDGE;
            matrix[b][a] = NO_EDGE;
            edgeCount--;
        }
        return exists;
    }

    /* Description: Prints the entire adjacency matrix using the building
    -              names. A "-" means that there is no connection.
    - Parameters: None
    - Return Value: None (void function)
    - Complexity: O(n^2)
     */
    void printMatrix() {
        cout << "\t";
        for (int j = 0; j < nodeCount; j++) {
            cout << nodes[j].name << "\t";
        }
        cout << endl;
        for (int i = 0; i < nodeCount; i++) {
            cout << nodes[i].name << "\t";
            for (int j = 0; j < nodeCount; j++) {
                if (matrix[i][j] == NO_EDGE) {
                    cout << "-\t";
                } else {
                    cout << matrix[i][j] << "\t";
                }
            }
            cout << endl;
        }
        cout << endl;
    }

    /* Description: Determines if the graph is connected by using
    -              BFS traversal from node 0 and checking if all
    -              nodes are visited.
    - Parameters: None
    - Return Value: bool (true if all nodes are reachable, if not false)
    - Complexity: O(n^2)
     */
    bool isConnected() {
        bool connected = true;
        if (nodeCount > 0) {
            bool* visited = new bool[nodeCount];
            for (int i = 0; i < nodeCount; i++) {
                visited[i] = false;
            }
            int count = 0;
            visited[0] = true;
            Queue q(nodeCount);
            q.enqueue(0);
            while (!q.isEmpty()) {
                int curr = q.dequeue();
                count++;
                for (int i = 0; i < nodeCount; i++) {
                    if (matrix[curr][i] != NO_EDGE && visited[i] == false) {
                        visited[i] = true;
                        q.enqueue(i);
                    }
                }
            }
            delete[] visited;
            connected = count == nodeCount;
        }
        return connected;
    }

    /* Description: Finds the connected components using BFS from every node
    -              that has not been visited yet.
    - Parameters: None
    - Return Value: vector<vector<int>> (each inner vector is one component,
    -               with its node indices in increasing order)
    - Complexity: O(n^2)
     */
    vector<vector<int>> getComponents() {
        vector<vector<int>> components;
        vector<bool> visited(nodeCount, false);
        for (int start = 0; start < nodeCount; start++) {
            if (visited[start] == false) {
                vector<int> component;
                Queue q(nodeCount);
                visited[start] = true;
                q.enqueue(start);
                while (!q.isEmpty()) {
                    int curr = q.dequeue();
                    component.push_back(curr);
                    for (int i = 0; i < nodeCount; i++) {
                        if (matrix[curr][i] != NO_EDGE && visited[i] == false) {
                            visited[i] = true;
                            q.enqueue(i);
                        }
                    }
                }
                sort(component.begin(), component.end());
                components.push_back(component);
            }
        }
        return components;
    }

    /* Description: Performs a Depth-First Search (DFS) starting from
    -              a given node using a Stack and prints the traversal order.
    -              A node is marked as visited when it is taken out of the
    -              stack. Neighbors are pushed in reverse order so the one
    -              with the smallest index is visited first.
    - Parameters: start - starting node index
    - Return Value: None (void function)
    - Complexity: O(n^2)
     */
    void dfs(int start) {
        if (!isValidIndex(start)) {
            cout << "  [Error] DFS: nodo inicial invalido." << endl;
        } else {
            bool* visited = new bool[nodeCount];
            for (int i = 0; i < nodeCount; i++) {
                visited[i] = false;
            }
            Stack s(nodeCount);
            s.push(start);
            while (!s.isEmpty()) {
                int curr = s.pop();
                if (visited[curr] == false) {
                    visited[curr] = true;
                    cout << nodes[curr].name << " ";
                    for (int i = nodeCount - 1; i >= 0; i--) {
                        if (matrix[curr][i] != NO_EDGE && visited[i] == false) {
                            s.push(i);
                        }
                    }
                }
            }
            cout << endl;
            delete[] visited;
        }
    }

    /* Description: Performs a Breadth-First Search (BFS) starting from a given
    -              node using a Queue and prints the traversal order. A node
    -              is marked as visited when it enters the queue.
    - Parameters: start - starting node index
    - Return Value: None (void function)
    - Complexity: O(n^2)
     */
    void bfs(int start) {
        if (!isValidIndex(start)) {
            cout << "  [Error] BFS: nodo inicial invalido." << endl;
        } else {
            bool* visited = new bool[nodeCount];
            for (int i = 0; i < nodeCount; i++) {
                visited[i] = false;
            }
            visited[start] = true;
            Queue q(nodeCount);
            q.enqueue(start);
            while (!q.isEmpty()) {
                int curr = q.dequeue();
                cout << nodes[curr].name << " ";
                for (int i = 0; i < nodeCount; i++) {
                    if (matrix[curr][i] != NO_EDGE && visited[i] == false) {
                        visited[i] = true;
                        q.enqueue(i);
                    }
                }
            }
            cout << endl;
            delete[] visited;
        }
    }

    /* Description: Runs Dijkstra's algorithm from a start node. Costs are
    -              never negative (addEdge validates it). The next node is
    -              the unprocessed node with the smallest finite distance; if
    -              none exists, the remaining nodes are unreachable and the
    -              algorithm stops. INFINITE_COST is never added to a cost.
    - Parameters: start - source node index
    -             distance - (output) minimum cost to each node, or INFINITE_COST
    -             from - (output) predecessor of each node, or -1
    - Return Value: None (void function)
    - Complexity: O(n^2)
     */
    void computeDijkstra(int start, vector<int>& distance, vector<int>& from) {
        distance.assign(nodeCount, INFINITE_COST);
        from.assign(nodeCount, -1);
        vector<bool> processed(nodeCount, false);
        if (isValidIndex(start)) {
            distance[start] = 0;
            bool hasNext = true;
            while (hasNext) {
                int u = -1;
                for (int j = 0; j < nodeCount; j++) {
                    if (processed[j] == false && distance[j] != INFINITE_COST) {
                        if (u == -1 || distance[j] < distance[u]) {
                            u = j;
                        }
                    }
                }
                if (u == -1) {
                    hasNext = false;
                } else {
                    processed[u] = true;
                    for (int k = 0; k < nodeCount; k++) {
                        if (matrix[u][k] != NO_EDGE && processed[k] == false) {
                            int newDist = distance[u] + matrix[u][k];
                            if (newDist < distance[k]) {
                                distance[k] = newDist;
                                from[k] = u;
                            }
                        }
                    }
                }
            }
        }
    }

    /* Description: Calculates the minimum cost paths from a given start
    -              node to all other nodes using Dijkstra's algorithm and
    -              prints them. Unreachable nodes are reported as such.
    - Parameters: start - source node index
    - Return Value: None (void function)
    - Complexity: O(n^2)
     */
    void dijkstra(int start) {
        if (!isValidIndex(start)) {
            cout << "  [Error] Dijkstra: nodo inicial invalido." << endl;
        } else {
            vector<int> distance;
            vector<int> from;
            computeDijkstra(start, distance, from);
            for (int i = 0; i < nodeCount; i++) {
                cout << "  " << nodes[start].name << " -> " << nodes[i].name << ": ";
                if (distance[i] == INFINITE_COST) {
                    cout << "inalcanzable" << endl;
                } else {
                    vector<int> path = buildPath(from, i);
                    for (int j = 0; j < (int) path.size(); j++) {
                        if (j > 0) {
                            cout << " -> ";
                        }
                        cout << nodes[path[j]].name;
                    }
                    cout << " (costo = " << distance[i] << ")" << endl;
                }
            }
        }
    }

    /* Description: Finds the minimum cost route between two nodes with
    -              Dijkstra's algorithm.
    - Parameters: start - origin node index
    -             endNode - destination node index
    -             path - (output) node indices of the route, empty if there is none
    - Return Value: int (cost of the route, or INFINITE_COST if there is no route)
    - Complexity: O(n^2)
     */
    int findRoute(int start, int endNode, vector<int>& path) {
        vector<int> distance;
        vector<int> from;
        int cost = INFINITE_COST;
        path.clear();
        if (isValidIndex(start) && isValidIndex(endNode)) {
            computeDijkstra(start, distance, from);
            cost = distance[endNode];
            if (cost != INFINITE_COST) {
                path = buildPath(from, endNode);
            }
        }
        return cost;
    }

    /* Description: Finds the edges of the Minimum Spanning Tree (MST) using
    -              Prim's algorithm starting from node 0. If the graph is
    -              disconnected, when the cheapest unprocessed node has
    -              INFINITE_COST it starts a new tree there, so the result is
    -              a minimum spanning forest and no invented edges are added.
    - Parameters: selectedEdges - (output) edges selected
    -             totalCost - (output) sum of the costs of the selected edges
    - Return Value: bool (true if the result connects all nodes, if not false)
    - Complexity: O(n^2)
     */
    bool primMST(vector<Edge>& selectedEdges, int& totalCost) {
        selectedEdges.clear();
        totalCost = 0;
        int treeCount = 0;
        if (nodeCount > 0) {
            int* distance = new int[nodeCount];
            bool* processed = new bool[nodeCount];
            int* from = new int[nodeCount];
            for (int i = 0; i < nodeCount; i++) {
                distance[i] = INFINITE_COST;
                processed[i] = false;
                from[i] = -1;
            }
            for (int i = 0; i < nodeCount; i++) {
                int u = -1;
                for (int j = 0; j < nodeCount; j++) {
                    if (processed[j] == false && (u == -1 || distance[j] < distance[u])) {
                        u = j;
                    }
                }
                if (distance[u] == INFINITE_COST) {
                    treeCount++;
                } else {
                    selectedEdges.push_back(makeEdge(from[u], u, distance[u]));
                    totalCost += distance[u];
                }
                processed[u] = true;
                for (int k = 0; k < nodeCount; k++) {
                    if (matrix[u][k] != NO_EDGE && processed[k] == false && matrix[u][k] < distance[k]) {
                        distance[k] = matrix[u][k];
                        from[k] = u;
                    }
                }
            }
            delete[] distance;
            delete[] processed;
            delete[] from;
        }
        return treeCount <= 1;
    }

    /* Description: Finds the edges of the Minimum Spanning Tree (MST)
    -              using Kruskal's algorithm. In each step it selects the
    -              minimum weight edge whose ends belong to different groups.
    -              Cycle detection: group[i] identifies the part of the
    -              network that contains node i; an edge whose ends have the
    -              same group would create a cycle, so it is never selected.
    -              If no valid edge remains before selecting n - 1 edges, the
    -              graph is disconnected and the result is a forest.
    - Parameters: selectedEdges - (output) edges selected, in selection order
    -             totalCost - (output) sum of the costs of the selected edges
    - Return Value: bool (true if the result connects all nodes, if not false)
    - Complexity: O(n^3)
     */
    bool kruskalMST(vector<Edge>& selectedEdges, int& totalCost) {
        selectedEdges.clear();
        totalCost = 0;
        int* group = new int[nodeCount];
        for (int i = 0; i < nodeCount; i++) {
            group[i] = i;
        }
        int selectedCount = 0;
        bool foundEdge = true;
        while (selectedCount < nodeCount - 1 && foundEdge) {
            int weight = INFINITE_COST;
            int u = -1;
            int v = -1;
            // j starts at i + 1 so each bidirectional edge is checked once.
            for (int i = 0; i < nodeCount; i++) {
                for (int j = i + 1; j < nodeCount; j++) {
                    if (matrix[i][j] != NO_EDGE && group[i] != group[j] && matrix[i][j] < weight) {
                        weight = matrix[i][j];
                        u = i;
                        v = j;
                    }
                }
            }
            if (u == -1) {
                foundEdge = false;
            } else {
                selectedEdges.push_back(makeEdge(u, v, weight));
                totalCost += weight;
                selectedCount++;
                int oldGroup = group[v];
                int newGroup = group[u];
                for (int i = 0; i < nodeCount; i++) {
                    if (group[i] == oldGroup) {
                        group[i] = newGroup;
                    }
                }
            }
        }
        delete[] group;
        return nodeCount <= 1 || selectedCount == nodeCount - 1;
    }
};

/* Description: Removes spaces, tabs and the '\r' character at the beginning
-              and at the end of a line (files saved on Windows end in "\r\n").
- Parameters: line - text to clean
- Return Value: string (clean line)
- Complexity: O(n)
 */
string trimLine(string line) {
    string spaces = " \t\r\n";
    string result = "";
    size_t first = line.find_first_not_of(spaces);
    if (first != string::npos) {
        size_t last = line.find_last_not_of(spaces);
        result = line.substr(first, last - first + 1);
    }
    return result;
}

/* Description: Opens the input file and keeps only the useful lines. Empty
-              lines and lines that start with '#' (section headers) are
-              ignored. It detects files saved in RTF format.
- Parameters: fileName - name of the file
-             lines - (output) useful lines, in order
- Return Value: bool (true if the file could be read as plain text, if not false)
- Complexity: O(n) where n is the number of lines
 */
bool readUsefulLines(string fileName, vector<string>& lines) {
    bool valid = true;
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "[Error] No se pudo abrir el archivo \"" << fileName << "\"." << endl;
        cout << "        Revisa el nombre y que el Working directory de CLion sea la carpeta del proyecto." << endl;
        valid = false;
    } else {
        string line = "";
        while (getline(file, line)) {
            string clean = trimLine(line);
            if (clean.rfind("{\\rtf", 0) == 0) {
                valid = false;
            }
            if (clean != "" && clean[0] != '#') {
                lines.push_back(clean);
            }
        }
        file.close();
        if (!valid) {
            cout << "[Error] El archivo esta en formato RTF, no en texto plano." << endl;
            cout << "        Guardalo como texto plano (TextEdit: Formato > Convertir a texto normal)." << endl;
        }
    }
    return valid;
}

/* Description: Checks that there is still a line to read; if not, it prints
-              an error that mentions the section.
- Parameters: lines - useful lines of the file
-             position - index of the line that will be read
-             section - name of the section, used in the message
- Return Value: bool (true if the line exists, if not false)
- Complexity: O(1)
 */
bool hasLine(vector<string>& lines, int position, string section) {
    bool exists = position < (int) lines.size();
    if (!exists) {
        cout << "[Error] Faltan datos en la seccion " << section;
        cout << ": el archivo tiene menos lineas que las declaradas." << endl;
    }
    return exists;
}

/* Description: Prints a standard error for a line that has an invalid format.
- Parameters: line - text of the line
-             expected - description of the expected format
- Return Value: None (void function)
- Complexity: O(1)
 */
void printFormatError(string line, string expected) {
    cout << "[Error] Linea invalida: \"" << line << "\". Se esperaba: " << expected << "." << endl;
    cout << "        Revisa tambien que las cantidades declaradas coincidan con los datos." << endl;
}

/* Description: Reads the first line: number of nodes and number of edges.
- Parameters: line - text of the line
-             nodeCount - (output) declared number of buildings
-             edgeCount - (output) declared number of connections
- Return Value: bool (true if the line is valid, if not false)
- Complexity: O(1)
 */
bool parseCountsLine(string line, int& nodeCount, int& edgeCount) {
    stringstream ss(line);
    string extra = "";
    bool valid = (ss >> nodeCount >> edgeCount) && !(ss >> extra);
    valid = valid && nodeCount >= 1 && edgeCount >= 0;
    if (!valid) {
        printFormatError(line, "<edificios (1 o mas)> <conexiones (0 o mas)>");
    }
    return valid;
}

/* Description: Reads a building line: name x y.
- Parameters: line - text of the line
-             node - (output) building read
- Return Value: bool (true if the line is valid, if not false)
- Complexity: O(1)
 */
bool parseNodeLine(string line, Node& node) {
    stringstream ss(line);
    string extra = "";
    bool valid = (ss >> node.name >> node.x >> node.y) && !(ss >> extra);
    if (!valid) {
        printFormatError(line, "<nombre> <x> <y>");
    }
    return valid;
}

/* Description: Reads a connection line: nameA nameB cost.
- Parameters: line - text of the line
-             edge - (output) connection read
- Return Value: bool (true if the line is valid, if not false)
- Complexity: O(1)
 */
bool parseEdgeLine(string line, NamedEdge& edge) {
    stringstream ss(line);
    string extra = "";
    bool valid = (ss >> edge.nameA >> edge.nameB >> edge.cost) && !(ss >> extra);
    valid = valid && edge.cost >= 0 && edge.cost <= MAX_COST;
    if (!valid) {
        printFormatError(line, "<edificio> <edificio> <costo entero entre 0 y 1000000>");
    }
    return valid;
}

/* Description: Reads the failed connection line: nameA nameB.
- Parameters: line - text of the line
-             nameA - (output) first building
-             nameB - (output) second building
- Return Value: bool (true if the line is valid, if not false)
- Complexity: O(1)
 */
bool parseFailedLine(string line, string& nameA, string& nameB) {
    stringstream ss(line);
    string extra = "";
    bool valid = (ss >> nameA >> nameB) && !(ss >> extra);
    if (!valid) {
        printFormatError(line, "<edificio> <edificio>");
    }
    return valid;
}

/* Description: Reads a line that contains only one integer (0 or greater).
- Parameters: line - text of the line
-             number - (output) number read
- Return Value: bool (true if the line is valid, if not false)
- Complexity: O(1)
 */
bool parseCountLine(string line, int& number) {
    stringstream ss(line);
    string extra = "";
    bool valid = (ss >> number) && !(ss >> extra) && number >= 0;
    if (!valid) {
        printFormatError(line, "<cantidad de conexiones nuevas (0 o mas)>");
    }
    return valid;
}

/* Description: Reads the whole input file in order: counts, nodes, edges,
-              failed connection, new node, number of new edges and new
-              edges. It validates the format and the declared quantities.
- Parameters: fileName - name of the file
-             data - (output) data read
- Return Value: bool (true if the file is valid, if not false)
- Complexity: O(n) where n is the number of lines
 */
bool readInputFile(string fileName, InputData& data) {
    vector<string> lines;
    bool valid = readUsefulLines(fileName, lines);
    int position = 0;
    int declaredNodes = 0;
    int declaredEdges = 0;
    int declaredNewEdges = 0;
    valid = valid && hasLine(lines, position, "inicial (cantidades)");
    valid = valid && parseCountsLine(lines[position], declaredNodes, declaredEdges);
    position++;
    int i = 0;
    while (valid && i < declaredNodes) {
        Node node;
        valid = hasLine(lines, position, "# NODES") && parseNodeLine(lines[position], node);
        data.initialNodes.push_back(node);
        position++;
        i++;
    }
    i = 0;
    while (valid && i < declaredEdges) {
        NamedEdge edge;
        valid = hasLine(lines, position, "# EDGES") && parseEdgeLine(lines[position], edge);
        data.initialEdges.push_back(edge);
        position++;
        i++;
    }
    valid = valid && hasLine(lines, position, "# FAILED CONNECTION");
    valid = valid && parseFailedLine(lines[position], data.failedNameA, data.failedNameB);
    position++;
    valid = valid && hasLine(lines, position, "# NEW NODE");
    valid = valid && parseNodeLine(lines[position], data.newNode);
    position++;
    valid = valid && hasLine(lines, position, "# NEW EDGES (cantidad)");
    valid = valid && parseCountLine(lines[position], declaredNewEdges);
    position++;
    i = 0;
    while (valid && i < declaredNewEdges) {
        NamedEdge edge;
        valid = hasLine(lines, position, "# NEW EDGES") && parseEdgeLine(lines[position], edge);
        data.newEdges.push_back(edge);
        position++;
        i++;
    }
    if (valid && position < (int) lines.size()) {
        cout << "[Error] El archivo tiene mas lineas de las declaradas. Primera linea sobrante: \"";
        cout << lines[position] << "\"." << endl;
        valid = false;
    }
    return valid;
}

/* Description: Adds the initial buildings to the graph, rejecting repeated names.
- Parameters: graph - graph of candidate connections
-             data - data read from the file
- Return Value: bool (true if all buildings were added, if not false)
- Complexity: O(n^3)
 */
bool addInitialNodes(Graph& graph, InputData& data) {
    bool valid = true;
    int i = 0;
    while (valid && i < (int) data.initialNodes.size()) {
        Node node = data.initialNodes[i];
        valid = graph.addNode(node.name, node.x, node.y);
        if (!valid) {
            cout << "[Error] El edificio \"" << node.name << "\" esta repetido." << endl;
        }
        i++;
    }
    return valid;
}

/* Description: Adds the initial connections to the graph. It validates that
-              both buildings exist, that the connection is not a loop and
-              that it is not repeated.
- Parameters: graph - graph of candidate connections
-             data - data read from the file
- Return Value: bool (true if all connections were added, if not false)
- Complexity: O(m * n) where m is the number of connections
 */
bool addInitialEdges(Graph& graph, InputData& data) {
    bool valid = true;
    int i = 0;
    while (valid && i < (int) data.initialEdges.size()) {
        NamedEdge edge = data.initialEdges[i];
        int a = graph.getIndex(edge.nameA);
        int b = graph.getIndex(edge.nameB);
        if (a == -1 || b == -1) {
            cout << "[Error] La conexion " << edge.nameA << " - " << edge.nameB;
            cout << " usa un edificio que no existe." << endl;
            valid = false;
        } else if (a == b) {
            cout << "[Error] La conexion " << edge.nameA << " - " << edge.nameB;
            cout << " une un edificio consigo mismo." << endl;
            valid = false;
        } else if (graph.hasEdge(a, b)) {
            cout << "[Error] La conexion " << edge.nameA << " - " << edge.nameB << " esta repetida." << endl;
            valid = false;
        } else {
            valid = graph.addEdge(a, b, edge.cost, false);
        }
        i++;
    }
    return valid;
}

/* Description: Validates the names used by the failed connection, the new
-              building and the new connections, before the simulation starts.
- Parameters: graph - graph with the initial buildings
-             data - data read from the file
- Return Value: bool (true if all names are valid, if not false)
- Complexity: O(m * n) where m is the number of new connections
 */
bool validateEventNames(Graph& graph, InputData& data) {
    bool valid = true;
    if (graph.getIndex(data.failedNameA) == -1 || graph.getIndex(data.failedNameB) == -1) {
        cout << "[Error] La conexion fallida " << data.failedNameA << " - " << data.failedNameB;
        cout << " usa un edificio que no existe." << endl;
        valid = false;
    }
    if (graph.getIndex(data.newNode.name) != -1) {
        cout << "[Error] El nuevo edificio \"" << data.newNode.name << "\" ya existe." << endl;
        valid = false;
    }
    for (int i = 0; i < (int) data.newEdges.size(); i++) {
        NamedEdge edge = data.newEdges[i];
        bool knownA = graph.getIndex(edge.nameA) != -1 || edge.nameA == data.newNode.name;
        bool knownB = graph.getIndex(edge.nameB) != -1 || edge.nameB == data.newNode.name;
        if (!knownA || !knownB || edge.nameA == edge.nameB) {
            cout << "[Error] La nueva conexion " << edge.nameA << " - " << edge.nameB;
            cout << " usa un edificio inexistente o une un edificio consigo mismo." << endl;
            valid = false;
        }
    }
    return valid;
}

/* Description: Builds the graph of candidate connections and validates all
-              the names of the file.
- Parameters: graph - (output) empty graph that will be filled
-             data - data read from the file
- Return Value: bool (true if the data is valid, if not false)
- Complexity: O(n^3 + m * n)
 */
bool buildCandidateGraph(Graph& graph, InputData& data) {
    bool valid = addInitialNodes(graph, data);
    valid = valid && addInitialEdges(graph, data);
    valid = valid && validateEventNames(graph, data);
    return valid;
}

/* Description: Copies all the buildings of a graph into a vector.
- Parameters: graph - graph that contains the buildings
-             nodes - (output) copy of the buildings
- Return Value: None (void function)
- Complexity: O(n)
 */
void copyNodes(Graph& graph, vector<Node>& nodes) {
    nodes.clear();
    for (int i = 0; i < graph.getNodeCount(); i++) {
        nodes.push_back(graph.getNode(i));
    }
}

/* Description: Creates the configuration of minimum total cost using
-              Kruskal over the available candidate connections.
- Parameters: graph - graph of available candidate connections
-             title - name of the configuration
-             config - (output) configuration created
- Return Value: None (void function)
- Complexity: O(n^3)
 */
void createMinimumConfiguration(Graph& graph, string title, Configuration& config) {
    config.title = title;
    config.exists = true;
    config.isComplete = graph.kruskalMST(config.edges, config.totalCost);
    copyNodes(graph, config.nodes);
}

/* Description: Loads the buildings and the installed connections of a
-              configuration into a graph, so that the graph algorithms
-              (BFS, DFS, Dijkstra) only use the installed network.
- Parameters: config - configuration to load
-             installed - (output) graph created with config.nodes.size() nodes
- Return Value: None (void function)
- Complexity: O(n + m)
 */
void loadInstalledGraph(Configuration& config, Graph& installed) {
    for (int i = 0; i < (int) config.nodes.size(); i++) {
        installed.setNode(i, config.nodes[i].name, config.nodes[i].x, config.nodes[i].y);
    }
    for (int i = 0; i < (int) config.edges.size(); i++) {
        installed.addEdge(config.edges[i].from, config.edges[i].to, config.edges[i].cost, false);
    }
}

/* Description: Recalculates the total cost and the connectivity of a
-              configuration from its list of edges.
- Parameters: config - configuration to update
- Return Value: None (void function)
- Complexity: O(n^2)
 */
void updateConfigurationStatus(Configuration& config) {
    config.totalCost = 0;
    for (int i = 0; i < (int) config.edges.size(); i++) {
        config.totalCost += config.edges[i].cost;
    }
    Graph installed(config.nodes.size());
    loadInstalledGraph(config, installed);
    config.isComplete = installed.isConnected();
}

/* Description: Checks if a list contains a connection (same ends).
- Parameters: edges - list of connections
-             edge - connection to search
- Return Value: bool (true if it is in the list, if not false)
- Complexity: O(m)
 */
bool containsEdge(vector<Edge>& edges, Edge edge) {
    bool found = false;
    int i = 0;
    while (i < (int) edges.size() && !found) {
        found = edges[i].from == edge.from && edges[i].to == edge.to;
        i++;
    }
    return found;
}

/* Description: Removes a connection from a list, if it is there.
- Parameters: edges - list of connections
-             edge - connection to remove
- Return Value: bool (true if it was removed, if not false)
- Complexity: O(m)
 */
bool removeEdgeFromList(vector<Edge>& edges, Edge edge) {
    vector<Edge> remaining;
    bool removed = false;
    for (int i = 0; i < (int) edges.size(); i++) {
        if (edges[i].from == edge.from && edges[i].to == edge.to) {
            removed = true;
        } else {
            remaining.push_back(edges[i]);
        }
    }
    edges = remaining;
    return removed;
}

/* Description: Returns a connection as text, for example "A - C (costo 4)".
- Parameters: nodes - buildings of the configuration
-             edge - connection
- Return Value: string (text of the connection)
- Complexity: O(1)
 */
string edgeToText(vector<Node>& nodes, Edge edge) {
    return nodes[edge.from].name + " - " + nodes[edge.to].name + " (costo " + to_string(edge.cost) + ")";
}

/* Description: Prints the connected components of a configuration, using
-              only its installed connections.
- Parameters: config - configuration
- Return Value: None (void function)
- Complexity: O(n^2)
 */
void printComponents(Configuration& config) {
    Graph installed(config.nodes.size());
    loadInstalledGraph(config, installed);
    vector<vector<int>> components = installed.getComponents();
    cout << "  Componentes conectadas (" << components.size() << "):" << endl;
    for (int i = 0; i < (int) components.size(); i++) {
        cout << "    Grupo " << i + 1 << ": { ";
        for (int j = 0; j < (int) components[i].size(); j++) {
            if (j > 0) {
                cout << ", ";
            }
            cout << config.nodes[components[i][j]].name;
        }
        cout << " }" << endl;
    }
}

/* Description: Prints the connections, the total cost, the connectivity
-              and the components of a configuration.
- Parameters: config - configuration to print
- Return Value: None (void function)
- Complexity: O(n^2)
 */
void printConfiguration(Configuration& config) {
    cout << "--- " << config.title << " ---" << endl;
    if (!config.exists) {
        cout << "  Esta configuracion no existe." << endl;
    } else {
        cout << "  Conexiones (" << config.edges.size() << "):" << endl;
        for (int i = 0; i < (int) config.edges.size(); i++) {
            cout << "    " << edgeToText(config.nodes, config.edges[i]) << endl;
        }
        if (config.isComplete) {
            cout << "  Costo total de la configuracion: " << config.totalCost << endl;
            cout << "  Estado: todo el campus esta conectado." << endl;
        } else {
            cout << "  Costo de las conexiones presentes: " << config.totalCost << endl;
            cout << "  Estado: NO todo el campus esta conectado. Este costo NO corresponde" << endl;
            cout << "          a una solucion completa." << endl;
        }
        printComponents(config);
    }
    cout << endl;
}

/* Description: Prints the connections added and removed between two
-              configurations, the cost of the new cables and both total costs.
- Parameters: oldConfig - previous configuration
-             newConfig - new configuration (its node list contains all
-                         the nodes of oldConfig, at the same indices)
- Return Value: None (void function)
- Complexity: O(m^2)
 */
void printChanges(Configuration& oldConfig, Configuration& newConfig) {
    int newCablesCost = 0;
    int addedCount = 0;
    int removedCount = 0;
    cout << "  Cambios de \"" << oldConfig.title << "\" a \"" << newConfig.title << "\":" << endl;
    cout << "    Conexiones agregadas:" << endl;
    for (int i = 0; i < (int) newConfig.edges.size(); i++) {
        if (!containsEdge(oldConfig.edges, newConfig.edges[i])) {
            cout << "      + " << edgeToText(newConfig.nodes, newConfig.edges[i]) << endl;
            newCablesCost += newConfig.edges[i].cost;
            addedCount++;
        }
    }
    if (addedCount == 0) {
        cout << "      (ninguna)" << endl;
    }
    cout << "    Conexiones retiradas:" << endl;
    for (int i = 0; i < (int) oldConfig.edges.size(); i++) {
        if (!containsEdge(newConfig.edges, oldConfig.edges[i])) {
            cout << "      - " << edgeToText(oldConfig.nodes, oldConfig.edges[i]) << endl;
            removedCount++;
        }
    }
    if (removedCount == 0) {
        cout << "      (ninguna)" << endl;
    }
    cout << "    Costo de los cables nuevos que se deben instalar: " << newCablesCost << endl;
    cout << "    Costo total: " << oldConfig.totalCost << " -> " << newConfig.totalCost << endl;
    cout << endl;
}

/* Description: Prints the candidate connections that are still available.
- Parameters: graph - graph of candidate connections
- Return Value: None (void function)
- Complexity: O(n^2)
 */
void printCandidateEdges(Graph& graph) {
    cout << "  Conexiones candidatas disponibles (" << graph.getEdgeCount() << "):" << endl;
    for (int i = 0; i < graph.getNodeCount(); i++) {
        for (int j = i + 1; j < graph.getNodeCount(); j++) {
            if (graph.hasEdge(i, j)) {
                cout << "    " << graph.getNode(i).name << " - " << graph.getNode(j).name;
                cout << " (costo " << graph.getCost(i, j) << ")" << endl;
            }
        }
    }
    cout << endl;
}

/* Description: Stage A and B. Shows the candidate network and calculates
-              the original installed network of minimum total cost.
- Parameters: candidates - graph of candidate connections
-             original - (output) original configuration
- Return Value: None (void function)
- Complexity: O(n^3)
 */
void runOriginalStage(Graph& candidates, Configuration& original) {
    cout << "========== ETAPA 1: RED ORIGINAL ==========" << endl;
    cout << "  Edificios: " << candidates.getNodeCount() << endl;
    for (int i = 0; i < candidates.getNodeCount(); i++) {
        Node node = candidates.getNode(i);
        cout << "    " << node.name << " (" << node.x << ", " << node.y << ")" << endl;
    }
    printCandidateEdges(candidates);
    createMinimumConfiguration(candidates, "1. Red original instalada", original);
    if (!original.isComplete) {
        cout << "  AVISO: no es posible conectar todos los edificios con las conexiones" << endl;
        cout << "  candidatas. Solo se obtiene un bosque (plan parcial de costo minimo)." << endl;
        cout << endl;
    }
    printConfiguration(original);
}

/* Description: Stage D. Simulates the failure of the connection of the file.
-              The connection is removed from the candidate graph, so it stays
-              unavailable for the rest of the simulation.
- Parameters: candidates - graph of candidate connections (it is modified)
-             data - data read from the file
-             original - original configuration
-             afterFailure - (output) configuration after the failure
- Return Value: bool (true if the failed connection existed, if not false)
- Complexity: O(n^2)
 */
bool runFailureStage(Graph& candidates, InputData& data, Configuration& original, Configuration& afterFailure) {
    cout << "========== ETAPA 2: FALLA DE UNA CONEXION ==========" << endl;
    int a = candidates.getIndex(data.failedNameA);
    int b = candidates.getIndex(data.failedNameB);
    bool failureApplied = candidates.hasEdge(a, b);
    afterFailure = original;
    afterFailure.title = "2. Red despues de la falla (antes de restaurar)";
    cout << "  Conexion que falla: " << data.failedNameA << " - " << data.failedNameB << endl;
    if (!failureApplied) {
        cout << "  La conexion no existe entre las candidatas: no hay nada que fallar." << endl;
    } else {
        Edge failed = makeEdge(a, b, candidates.getCost(a, b));
        candidates.removeEdge(a, b);
        if (removeEdgeFromList(afterFailure.edges, failed)) {
            cout << "  La conexion existia y ESTABA INSTALADA. Se retira de la red." << endl;
        } else {
            cout << "  La conexion existia, pero NO estaba instalada (era solo candidata)." << endl;
            cout << "  La red instalada no cambia." << endl;
        }
        cout << "  A partir de ahora queda DANADA y no disponible." << endl;
    }
    updateConfigurationStatus(afterFailure);
    cout << endl;
    printConfiguration(afterFailure);
    cout << "  Todo el campus sigue conectado: " << yesNoText(afterFailure.isComplete) << endl;
    cout << endl;
    return failureApplied;
}

/* Description: Stage E. Calculates a new configuration of minimum total cost
-              with the connections that are still available and compares it
-              with the original configuration.
- Parameters: candidates - graph of available candidate connections
-             original - original configuration
-             restored - (output) restored configuration (exists = false if impossible)
- Return Value: None (void function)
- Complexity: O(n^3)
 */
void runRestorationStage(Graph& candidates, Configuration& original, Configuration& restored) {
    cout << "========== ETAPA 3: RESTAURACION ==========" << endl;
    cout << "  Interpretacion: se recalcula el plan completo con todas las conexiones" << endl;
    cout << "  disponibles (sin la danada). Se permite retirar o sustituir conexiones" << endl;
    cout << "  que ya estaban instaladas si eso da un costo total menor." << endl;
    cout << endl;
    Configuration attempt;
    createMinimumConfiguration(candidates, "3. Red restaurada", attempt);
    if (attempt.isComplete) {
        restored = attempt;
        printConfiguration(restored);
        printChanges(original, restored);
    } else {
        restored.title = "3. Red restaurada";
        restored.exists = false;
        cout << "  RESTAURACION IMPOSIBLE: con las conexiones disponibles no se puede" << endl;
        cout << "  conectar todo el campus. Grupos que no pueden unirse:" << endl;
        printComponents(attempt);
        cout << endl;
    }
}

/* Description: Checks if a new connection is the damaged connection.
- Parameters: edge - new connection
-             data - data read from the file
-             failureApplied - true if the damaged connection existed
- Return Value: bool (true if it is the damaged connection, if not false)
- Complexity: O(1)
 */
bool isDamagedEdge(NamedEdge edge, InputData& data, bool failureApplied) {
    bool sameDirection = edge.nameA == data.failedNameA && edge.nameB == data.failedNameB;
    bool otherDirection = edge.nameA == data.failedNameB && edge.nameB == data.failedNameA;
    return failureApplied && (sameDirection || otherDirection);
}

/* Description: Adds the new building and its possible connections to the
-              candidate graph. The damaged connection is never added again.
- Parameters: candidates - graph of candidate connections (it is modified)
-             data - data read from the file
-             failureApplied - true if the damaged connection existed
- Return Value: None (void function)
- Complexity: O(n^2 + m * n)
 */
void addNewBuilding(Graph& candidates, InputData& data, bool failureApplied) {
    candidates.addNode(data.newNode.name, data.newNode.x, data.newNode.y);
    cout << "  Nuevo edificio: " << data.newNode.name << " (" << data.newNode.x;
    cout << ", " << data.newNode.y << ")" << endl;
    cout << "  Nuevas conexiones posibles:" << endl;
    if (data.newEdges.size() == 0) {
        cout << "    (ninguna)" << endl;
    }
    for (int i = 0; i < (int) data.newEdges.size(); i++) {
        NamedEdge edge = data.newEdges[i];
        int a = candidates.getIndex(edge.nameA);
        int b = candidates.getIndex(edge.nameB);
        cout << "    " << edge.nameA << " - " << edge.nameB << " (costo " << edge.cost << "): ";
        if (isDamagedEdge(edge, data, failureApplied)) {
            cout << "IGNORADA, es la conexion danada y sigue fuera de servicio." << endl;
        } else if (candidates.hasEdge(a, b)) {
            cout << "IGNORADA, esa conexion ya existe entre las candidatas." << endl;
        } else {
            candidates.addEdge(a, b, edge.cost, false);
            cout << "agregada como candidata." << endl;
        }
    }
    cout << endl;
}

/* Description: Stage F. Adds the new building, recalculates the plan of
-              minimum total cost with all available connections and compares
-              it with the previous installed network.
- Parameters: candidates - graph of candidate connections (it is modified)
-             data - data read from the file
-             previous - last installed configuration (restored, or after failure)
-             expanded - (output) configuration after the expansion
-             failureApplied - true if the damaged connection existed
- Return Value: None (void function)
- Complexity: O(n^3)
 */
void runExpansionStage(Graph& candidates, InputData& data, Configuration& previous, Configuration& expanded, bool failureApplied) {
    cout << "========== ETAPA 4: EXPANSION ==========" << endl;
    addNewBuilding(candidates, data, failureApplied);
    int newIndex = candidates.getIndex(data.newNode.name);
    int newEdgeCount = 0;
    for (int i = 0; i < candidates.getNodeCount(); i++) {
        if (candidates.hasEdge(newIndex, i)) {
            newEdgeCount++;
        }
    }
    createMinimumConfiguration(candidates, "4. Red despues de agregar " + data.newNode.name, expanded);
    printConfiguration(expanded);
    if (newEdgeCount == 0) {
        cout << "  AVISO: el edificio " << data.newNode.name << " no tiene conexiones disponibles;" << endl;
        cout << "  no puede conectarse al campus." << endl;
    }
    if (!expanded.isComplete) {
        cout << "  AVISO: despues de la expansion NO es posible conectar todo el campus." << endl;
    }
    printChanges(previous, expanded);
    if (failureApplied) {
        int a = candidates.getIndex(data.failedNameA);
        int b = candidates.getIndex(data.failedNameB);
        bool inPlan = containsEdge(expanded.edges, makeEdge(a, b, 0));
        cout << "  Verificacion de la conexion danada " << data.failedNameA << " - " << data.failedNameB << ":" << endl;
        cout << "    Disponible entre las candidatas: ";
        if (candidates.hasEdge(a, b)) {
            cout << "SI (ERROR)" << endl;
        } else {
            cout << "NO" << endl;
        }
        cout << "    Usada en la configuracion final: ";
        if (inPlan) {
            cout << "SI (ERROR)" << endl;
        } else {
            cout << "NO" << endl;
        }
        cout << endl;
    }
}

/* Description: Prints a short summary of one configuration.
- Parameters: config - configuration
- Return Value: None (void function)
- Complexity: O(n^2)
 */
void printSummary(Configuration& config) {
    cout << "  " << config.title << endl;
    if (!config.exists) {
        cout << "    No existe (no fue posible obtenerla)." << endl;
    } else {
        cout << "    Conexiones: ";
        for (int i = 0; i < (int) config.edges.size(); i++) {
            if (i > 0) {
                cout << ", ";
            }
            cout << config.nodes[config.edges[i].from].name << "-" << config.nodes[config.edges[i].to].name;
            cout << "(" << config.edges[i].cost << ")";
        }
        cout << endl;
        Graph installed(config.nodes.size());
        loadInstalledGraph(config, installed);
        cout << "    Edificios: " << config.nodes.size();
        cout << " | Componentes: " << installed.getComponents().size();
        if (config.isComplete) {
            cout << " | Conectada: SI | Costo total: " << config.totalCost << endl;
        } else {
            cout << " | Conectada: NO | Costo de conexiones presentes: " << config.totalCost;
            cout << " (no es solucion completa)" << endl;
        }
    }
}

/* Description: Stage G. Compares the four saved configurations and explains
-              the changes between consecutive states.
- Parameters: states - array with the STATE_COUNT configurations
- Return Value: None (void function)
- Complexity: O(n^2 + m^2)
 */
void printComparison(Configuration states[]) {
    cout << "========== ETAPA 5: COMPARACION DE CONFIGURACIONES ==========" << endl;
    for (int i = 0; i < STATE_COUNT; i++) {
        printSummary(states[i]);
    }
    cout << endl;
    cout << "  Explicacion de los cambios:" << endl;
    printChanges(states[0], states[1]);
    if (states[2].exists) {
        printChanges(states[1], states[2]);
        printChanges(states[2], states[3]);
    } else {
        cout << "  La restauracion no existe; la expansion se compara con la red despues de la falla." << endl;
        printChanges(states[1], states[3]);
    }
}

/* Description: Reads one line from the keyboard and cleans it. If the input
-              ends, it returns "0" so the menu finishes.
- Parameters: message - text shown to the user
- Return Value: string (line written by the user)
- Complexity: O(n)
 */
string readText(string message) {
    string text = "";
    cout << message;
    if (getline(cin, text)) {
        text = trimLine(text);
    } else {
        text = "0";
    }
    return text;
}

/* Description: Asks the user which configuration to use.
- Parameters: states - array with the STATE_COUNT configurations
- Return Value: int (index 0..3 of an existing configuration, or -1 if invalid)
- Complexity: O(1)
 */
int selectConfiguration(Configuration states[]) {
    int selected = -1;
    cout << "  Configuraciones:" << endl;
    for (int i = 0; i < STATE_COUNT; i++) {
        cout << "    " << states[i].title;
        if (!states[i].exists) {
            cout << " (no existe)";
        }
        cout << endl;
    }
    string option = readText("  Elige el numero de configuracion (1-4): ");
    if (option == "1" || option == "2" || option == "3" || option == "4") {
        selected = stoi(option) - 1;
        if (!states[selected].exists) {
            cout << "  Esa configuracion no existe." << endl;
            selected = -1;
        }
    } else {
        cout << "  Opcion invalida." << endl;
    }
    return selected;
}

/* Description: Asks for two buildings and prints the route between them
-              using only the connections installed in the chosen
-              configuration (Dijkstra over that installed network).
- Parameters: states - array with the STATE_COUNT configurations
- Return Value: None (void function)
- Complexity: O(n^2)
 */
void queryRoute(Configuration states[]) {
    int selected = selectConfiguration(states);
    if (selected != -1) {
        Graph installed(states[selected].nodes.size());
        loadInstalledGraph(states[selected], installed);
        string originName = readText("  Edificio de origen: ");
        string destinationName = readText("  Edificio de destino: ");
        int origin = installed.getIndex(originName);
        int destination = installed.getIndex(destinationName);
        if (origin == -1 || destination == -1) {
            cout << "  Error: el edificio ";
            if (origin == -1) {
                cout << "\"" << originName << "\" ";
            }
            if (destination == -1) {
                cout << "\"" << destinationName << "\" ";
            }
            cout << "no existe en esta configuracion." << endl;
        } else if (origin == destination) {
            cout << "  Consulta de un edificio hacia si mismo: ruta " << originName << ", costo 0." << endl;
            cout << "  No se necesita ninguna conexion." << endl;
        } else {
            vector<int> path;
            int cost = installed.findRoute(origin, destination, path);
            if (cost == INFINITE_COST) {
                cout << "  No hay ruta: " << originName << " y " << destinationName;
                cout << " estan en componentes distintas de esta red instalada." << endl;
            } else {
                cout << "  Ruta: ";
                for (int i = 0; i < (int) path.size(); i++) {
                    if (i > 0) {
                        cout << " -> ";
                    }
                    cout << installed.getNode(path[i]).name;
                }
                cout << endl;
                cout << "  Suma de los costos de instalacion de la ruta: " << cost << endl;
                cout << "  (Es costo de instalacion de cables; el archivo no da latencia ni tiempo.)" << endl;
            }
        }
    }
    cout << endl;
}

/* Description: Prints the BFS and DFS traversals and the Dijkstra routes
-              from a building, over the installed network of a configuration.
- Parameters: states - array with the STATE_COUNT configurations
- Return Value: None (void function)
- Complexity: O(n^2)
 */
void showTraversals(Configuration states[]) {
    int selected = selectConfiguration(states);
    if (selected != -1) {
        Graph installed(states[selected].nodes.size());
        loadInstalledGraph(states[selected], installed);
        string startName = readText("  Edificio inicial: ");
        int start = installed.getIndex(startName);
        if (start == -1) {
            cout << "  Error: el edificio \"" << startName << "\" no existe en esta configuracion." << endl;
        } else {
            cout << "  BFS desde " << startName << ": ";
            installed.bfs(start);
            cout << "  DFS desde " << startName << ": ";
            installed.dfs(start);
            cout << "  Rutas de costo minimo desde " << startName << " (Dijkstra):" << endl;
            installed.dijkstra(start);
        }
    }
    cout << endl;
}

/* Description: Tests the Stack class: growth, order and empty operations.
- Parameters: None
- Return Value: None (void function)
- Complexity: O(n)
 */
void testStack() {
    cout << "  PRUEBA DE STACK (capacidad inicial 2)" << endl;
    Stack s(2);
    for (int i = 1; i <= 5; i++) {
        s.push(i * 10);
        cout << "    push(" << i * 10 << ") -> tamano " << s.getSize() << ", capacidad " << s.getCapacity() << endl;
    }
    cout << "    getTop() = " << s.getTop() << endl;
    cout << "    pop en orden: ";
    while (!s.isEmpty()) {
        cout << s.pop() << " ";
    }
    cout << endl;
    cout << "    Operaciones con la pila vacia:" << endl;
    int value = s.pop();
    cout << "    pop() devolvio " << value << " (EMPTY_VALUE)" << endl;
    value = s.getTop();
    cout << "    getTop() devolvio " << value << " (EMPTY_VALUE)" << endl;
    cout << endl;
}

/* Description: Tests the Queue class: circular movement, growth after the
-              circular movement, order and empty operations.
- Parameters: None
- Return Value: None (void function)
- Complexity: O(n)
 */
void testQueue() {
    cout << "  PRUEBA DE QUEUE (capacidad inicial 3)" << endl;
    Queue q(3);
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    cout << "    enqueue 1, 2, 3 -> front " << q.getFrontPosition() << ", end " << q.getEndPosition();
    cout << ", capacidad " << q.getCapacity() << endl;
    int first = q.dequeue();
    int second = q.dequeue();
    cout << "    dequeue -> " << first << ", " << second << endl;
    q.enqueue(4);
    q.enqueue(5);
    cout << "    enqueue 4, 5 (avance circular) -> front " << q.getFrontPosition();
    cout << ", end " << q.getEndPosition() << ", capacidad " << q.getCapacity() << endl;
    q.enqueue(6);
    cout << "    enqueue 6 (cola llena, crece) -> front " << q.getFrontPosition();
    cout << ", end " << q.getEndPosition() << ", capacidad " << q.getCapacity() << endl;
    q.enqueue(7);
    cout << "    enqueue 7 -> tamano " << q.getSize() << ", getFront() = " << q.getFront() << endl;
    cout << "    dequeue en orden (debe ser 3 4 5 6 7): ";
    while (!q.isEmpty()) {
        cout << q.dequeue() << " ";
    }
    cout << endl;
    cout << "    Operaciones con la cola vacia:" << endl;
    int value = q.dequeue();
    cout << "    dequeue() devolvio " << value << " (EMPTY_VALUE)" << endl;
    value = q.getFront();
    cout << "    getFront() devolvio " << value << " (EMPTY_VALUE)" << endl;
    cout << endl;
}

/* Description: Tests the graph algorithms on a small disconnected graph and
-              compares Prim with Kruskal on the current candidate network
-              (both must give the same total cost).
- Parameters: candidates - graph of current candidate connections
- Return Value: None (void function)
- Complexity: O(n^3)
 */
void testGraphAlgorithms(Graph& candidates) {
    cout << "  PRUEBA DE RECORRIDOS EN UN GRAFO DESCONECTADO" << endl;
    Graph small(5);
    small.setNode(0, "P", 0, 0);
    small.setNode(1, "Q", 1, 0);
    small.setNode(2, "R", 2, 0);
    small.setNode(3, "S", 3, 0);
    small.setNode(4, "T", 4, 0);
    small.addEdge(0, 1, 0, false);
    small.addEdge(1, 2, 5, false);
    small.addEdge(3, 4, 1, false);
    cout << "    Conexiones: P-Q(0), Q-R(5), S-T(1). Nota: P-Q tiene costo 0 y SI existe." << endl;
    cout << "    BFS desde P: ";
    small.bfs(0);
    cout << "    DFS desde P: ";
    small.dfs(0);
    cout << "    BFS desde S: ";
    small.bfs(3);
    cout << "    BFS desde indice invalido 9: ";
    small.bfs(9);
    cout << "    isConnected(): " << yesNoText(small.isConnected()) << endl;
    cout << "    Rutas desde P (Dijkstra):" << endl;
    small.dijkstra(0);
    vector<Edge> primEdges;
    vector<Edge> kruskalEdges;
    int primCost = 0;
    int kruskalCost = 0;
    bool primComplete = small.primMST(primEdges, primCost);
    bool kruskalComplete = small.kruskalMST(kruskalEdges, kruskalCost);
    cout << "    Grafo desconectado -> Prim: " << primEdges.size() << " conexiones, costo " << primCost;
    cout << ", conecta todo: " << yesNoText(primComplete) << endl;
    cout << "    Grafo desconectado -> Kruskal: " << kruskalEdges.size() << " conexiones, costo " << kruskalCost;
    cout << ", conecta todo: " << yesNoText(kruskalComplete) << endl;
    cout << endl;
    cout << "  COMPARACION PRIM VS KRUSKAL EN LA RED CANDIDATA ACTUAL" << endl;
    primComplete = candidates.primMST(primEdges, primCost);
    kruskalComplete = candidates.kruskalMST(kruskalEdges, kruskalCost);
    cout << "    Prim: costo " << primCost << ", conecta todo: " << yesNoText(primComplete) << endl;
    cout << "    Kruskal: costo " << kruskalCost << ", conecta todo: " << yesNoText(kruskalComplete) << endl;
    if (primCost == kruskalCost) {
        cout << "    Ambos obtienen el mismo costo total." << endl;
    } else {
        cout << "    ERROR: los costos son distintos." << endl;
    }
    cout << endl;
}

/* Description: Shows the menu of queries until the user chooses 0.
- Parameters: states - array with the STATE_COUNT configurations
-             candidates - graph of current candidate connections
- Return Value: None (void function)
- Complexity: depends on the options chosen
 */
void runMenu(Configuration states[], Graph& candidates) {
    string option = "";
    while (option != "0") {
        cout << "========== MENU ==========" << endl;
        cout << "  1. Consultar ruta entre dos edificios" << endl;
        cout << "  2. Recorridos BFS, DFS y rutas desde un edificio" << endl;
        cout << "  3. Mostrar matriz de conexiones candidatas disponibles" << endl;
        cout << "  4. Pruebas de las clases propias (Stack, Queue, recorridos, Prim vs Kruskal)" << endl;
        cout << "  0. Salir" << endl;
        option = readText("  Opcion: ");
        cout << endl;
        if (option == "1") {
            queryRoute(states);
        } else if (option == "2") {
            showTraversals(states);
        } else if (option == "3") {
            candidates.printMatrix();
        } else if (option == "4") {
            testStack();
            testQueue();
            testGraphAlgorithms(candidates);
        } else if (option != "0") {
            cout << "  Opcion invalida." << endl;
            cout << endl;
        }
    }
    cout << "Fin del programa." << endl;
}

/* Description: Runs all the stages of the simulation in order and then the menu.
- Parameters: candidates - graph of candidate connections
-             data - data read from the file
- Return Value: None (void function)
- Complexity: O(n^3)
 */
void runSimulation(Graph& candidates, InputData& data) {
    Configuration states[STATE_COUNT];
    runOriginalStage(candidates, states[0]);
    bool failureApplied = runFailureStage(candidates, data, states[0], states[1]);
    runRestorationStage(candidates, states[0], states[2]);
    if (states[2].exists) {
        runExpansionStage(candidates, data, states[2], states[3], failureApplied);
    } else {
        runExpansionStage(candidates, data, states[1], states[3], failureApplied);
    }
    printComparison(states);
    runMenu(states, candidates);
}

/* Description: Main function. Asks for the input file, reads and validates
-              it, and runs the simulation.
- Parameters: None
- Return Value: int (0 if the program finished correctly, 1 if the file was invalid)
- Complexity: O(n^3)
 */
int main() {
    int exitCode = 0;
    cout << "===== CAMPUS CONNECT! =====" << endl;
    string fileName = readText("Nombre del archivo de entrada (Enter = " + DEFAULT_FILE_NAME + "): ");
    if (fileName == "" || fileName == "0") {
        fileName = DEFAULT_FILE_NAME;
    }
    cout << endl;
    InputData data;
    Graph candidates(0);
    if (readInputFile(fileName, data) && buildCandidateGraph(candidates, data)) {
        runSimulation(candidates, data);
    } else {
        cout << "No se puede continuar porque el archivo de entrada tiene errores." << endl;
        exitCode = 1;
    }
    return exitCode;
}
