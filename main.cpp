
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

const string DEFAULT_FILE_NAME = "test_fix.txt";

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
    char name;
    int x = 0;
    int y = 0;

    Node();
    Node(char name, int x, int y);
};

/* Description: the basic Constructor for the Node class with no parameters given.
-               It sets the name to a "non-existing" state with '-' and leaves x and y as 0.
- Parameters: None
- Return Value: None
- Complexity: O(1)
*/
Node::Node(){
    name = '-';
}

/* Description: the basic Constructor for the Node class with parameters given.
-               It sets the name, x and y of the Node.
- Parameters: name - the character that represents the node
-             x - the (x) coordenate of the position of the node
-             y - the (y) coordenate of the position of the node
- Return Value: None
- Complexity: O(1)
*/
Node::Node(char name, int x, int y){
    this->name = name;
    this->x = x;
    this->y = y;
}

// Represents one bidirectional connection between two node indices.
// It is always stored with from < to, so A-B and B-A are the same connection.
class Edge {
public:
    int from = -1;
    int to = -1;
    int cost = 0;

    Edge();
    Edge(int from, int to, int cost);
};

/* Description: the basic Constructor for the Edge class with no parameters given.
-               It does nothing since the class already sets all 3 int variables from, to, and cost to their "null" state.
- Parameters: None
- Return Value: None
- Complexity: O(1)
*/
Edge::Edge(){
    // Does nothing, default values are already given.
}

/* Description: the basic Constructor for the Edge class with parameters given.
-               It sets from to the minimum value between the parameters "from" and "to", and sets to to the maximum,
-               cost is set to the parameter "cost".

- Parameters: from - the Node index the edge comes from
-             to - the Node index the edge ends on
-             cost - the weight it takes to reach the "to" Node comming from the "from" Node
- Return Value: None
- Complexity: O(1)
*/
Edge::Edge(int from, int to, int cost){
    this->from = min(from, to);
    this->to = max(from, to);
    this->cost = cost;
}

// Connection read from the file, still written with building names.
/*  Note: Currently not used. The purpose wasn't seen as necesarry and
    was replaced with more class Edge variables. */
class NamedEdge {
public:
    string nameA = "";
    string nameB = "";
    int cost = 0;
};

// All the data read from the input file.
class InputData {
public:
    int N, E;
    vector<Node> initialNodes;
    vector<Edge> initialEdges;
    Edge failedEdge;
    Node newNode;
    vector<Edge> newEdges;
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
    int getIndex(char name) {
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
    bool setNode(int index, char name, int x, int y) {
        bool valid = isValidIndex(index);
        if (valid) {
            nodes[index].name = name;
            nodes[index].x = x;
            nodes[index].y = y;
        }
        return valid;
    }
};
  
// void runSimulation(Graph& candidates, InputData& data) {
//     Configuration states[STATE_COUNT];
//     runOriginalStage(candidates, states[0]);
//     bool failureApplied = runFailureStage(candidates, data, states[0], states[1]);
//     runRestorationStage(candidates, states[0], states[2]);
//     if (states[2].exists) {
//         runExpansionStage(candidates, data, states[2], states[3], failureApplied);
//     } else {
//         runExpansionStage(candidates, data, states[1], states[3], failureApplied);
//     }
//     printComparison(states);
//     runMenu(states, candidates);
// }

/* Description: Searches for a node by its character name among the initial nodes
-              (indices 0 to N - 1) and the new expansion node (index N).
- Parameters: data - reference to the InputData class containing the nodes
-             a - character name of the node to search for
- Return Value: int (index of the node if found, N if it matches newNode, or -1 if it does not exist)
- Complexity: O(N)
 */
int indexSearcher(InputData &data, char a) {
    int index = -1;
    if (data.newNode.name == a) { index = data.N;}
    int i = 0;
    while (i < data.N && index == -1) {
        if (data.initialNodes[i].name == a) {
            index = i;
        }
        i++;
    }
    return index;
}

/* Description: Computes the MST using Kruskal's
-              algorithm. Sorts the candidate edges by cost in ascending order
-              and iteratively selects the cheapest edges that do not form a
-              cycle using Union-Find.
- Parameters: edgeList - vector of candidate Edge objects to build the MST from
-             nodeCount - total number of nodes in the graph
- Return Value: Configuration (contains the selected MST edges, totalCost, and
-               isComplete set to true if all nodeCount nodes are connected)
- Complexity: O(E^2 + E * N)
 */
Configuration kruskalMST(vector<Edge> edgeList, int nodeCount) {
    Configuration outputConfig;
    // BubleSort the edges
    int E = edgeList.size();
    for (int i = 0; i < E-1; i++) {
        for (int j = 0; j < E-i-1; j++) {
            if (edgeList[j].cost > edgeList[j+1].cost) {
                Edge aux = edgeList[j];
                edgeList[j] = edgeList[j+1];
                edgeList[j+1] = aux;
            }
        }
    }
    int k = 0;
    int nSelected = 0;
    int* parent = new int[nodeCount];
    for (int i = 0; i < nodeCount; i++) {
        parent[i] = i;
    }
    int node1 = -1, node2 = -1, node1leader = -1, node2leader = -1;
    while (nSelected < nodeCount-1 && k < E) {
        node1 = edgeList[k].from;
        node2 = edgeList[k].to;
        node1leader = parent[node1];
        node2leader = parent[node2];
        while(parent[node2leader] != node2leader) {
            node2leader = parent[node2leader];
        }
        while (parent[node1leader] != node1leader) {
            node1leader = parent[node1leader];
        }
        if (node2leader != node1leader) {
            outputConfig.edges.push_back(edgeList[k]);
            parent[node2leader] = node1leader;
            nSelected++;
            outputConfig.totalCost += edgeList[k].cost;
        }
        k++;
    }
    if (nSelected == nodeCount -1) {
        outputConfig.isComplete = true;
    }
    delete[] parent;
    return outputConfig;
}

/* Description: Opens and reads the input file, parsing and validating all
-              simulation data: initial node and edge counts (N, E), initial
-              nodes, initial edges, the failed connection edge, the new
-              expansion node, and the candidate new edges.
- Parameters: fileName - path or name of the input file to read
-             data - reference to the InputData class where parsed data is stored
- Return Value: bool (true if the file was opened, read, and validated without errors, false otherwise)
- Complexity: O(N * E)
 */
bool readInputFile(string fileName,InputData &data) {
    string line;
    // node info
    char letter;
    int x, y;
    // edge info
    char a, b;
    int w;
    ifstream inFile(fileName);

    if (inFile.is_open()) {
        getline(inFile, line);
        istringstream header(line);
        header >> data.N >> data.E;

        cout << "Nodes N: " << data.N << endl;
        cout << "Edges E: " << data.E << endl << endl;

        // currently at the summary of initial Nodes and Edges
        getline(inFile, line);
        // Currently at an empty line \n
        getline(inFile, line);
        // Currently at header "# NODES"

        for (int i = 0; i < data.N; i++) {
            getline(inFile, line);
            istringstream node(line);
            node >> letter >> x >> y;
            data.initialNodes.push_back(Node(letter, x, y));
        }

        // Currently at last initial Node
        getline(inFile, line);
        // Currently at an empty line \n
        getline(inFile, line);
        // Currently at header "# EDGES"

        for (int i = 0; i < data.E; i++) {
            getline(inFile, line);
            istringstream edge(line);
            edge >> a >> b;
            int indexa = indexSearcher(data, a);
            int indexb = indexSearcher(data, b);
            if (indexa == -1 or indexb == -1) {
                cout << "[Error] readInputFile/initialEdges: Non-existent node." << endl;
                return false;
            } else if(indexa == indexb) {
                cout << "[Error] readInputFile/initialEdges: The nodes were the same." << endl;
                return false;
            }
            if (!(edge >> w)) {
                cout << "[Error] readInputFile/initialEdges: The weight was not listed." << endl;
                return false;
            } else {
                if (w < 0) {
                    cout << "[Error] readInputFile/initialEdges: The weight can't be negative." << endl;
                    return false;
                }
                else {
                    data.initialEdges.push_back(Edge(indexa, indexb, w));
                }
            }
        }

        // Currently at last initial Edge
        getline(inFile, line);
        // Currently at an empty line \n
        getline(inFile, line);
        // Currently at header "# FAILED CONNECTION"
        getline(inFile, line);
        // Currently at the Failed Connection Edge

        istringstream fcedge(line);
        fcedge >> a >> b;
        int indexa = indexSearcher(data, a);
        int indexb = indexSearcher(data, b);
        if (indexa == -1 or indexb == -1) {
            cout << "[Error] readInputFile/failedConnection: Non-existent node." << endl;
            return false;
        } else if(indexa == indexb) {
            cout << "[Error] readInputFile/failedConnection: The nodes were the same." << endl;
            return false;
        }

        data.failedEdge.from = min(indexa, indexb);
        data.failedEdge.to = max(indexa, indexb);
        // Currently at the Failed Connection Edge
        getline(inFile, line);
        // Currently at an empty line \n
        getline(inFile, line);
        // Currently at header "# NEW NODE"
        getline(inFile, line);
        // Currently at the New Node
        istringstream node(line);
        node >> letter >> x >> y;
        data.newNode.name = letter;
        data.newNode.x = x;
        data.newNode.y = y;

        // Currently at the New Node
        getline(inFile, line);
        // Currently at an empty line \n
        getline(inFile, line);
        // Currently at header "# NEW EDGES"
        getline(inFile, line);
        // Currently at the amount of new edges
        istringstream newedgeheader(line);
        int E;
        newedgeheader >> E;
        for (int i = 0; i < E; i++) {
            getline(inFile, line);
            istringstream edge2(line);
            edge2 >> a >> b;
            int indexa = indexSearcher(data, a);
            int indexb = indexSearcher(data, b);
            if (indexa == -1 or indexb == -1) {
                cout << "[Error] readInputFile/newEdge: Non-existent node." << endl;
                return false;
            } else if(indexa == indexb) {
                cout << "[Error] readInputFile/newEdge: The nodes were the same." << endl;
                return false;
            }
            if (!(edge2 >> w)) {
                cout << "[Error] readInputFile/newEdge: The weight was not listed." << endl;
                return false;
            } else {
                if (w < 0) {
                    cout << "[Error] readInputFile/newEdge: The weight can't be negative." << endl;
                    return false;
                }
                else {
                    data.newEdges.push_back(Edge(indexa, indexb, w));
                }
            }
        }
    }
    else {
        // File was not found
        cout << "[Error] readInputFile: File not found" << endl;
        return false;
    }
    // Debug (quitar antes de entregar): Replicate the test_fix.txt file only using InputData data info.
    cout << "--- test_fix.txt ---" << endl;
    cout << data.N << " " << data.E << endl << endl << "# NODES" << endl;
    for (int i = 0; i < data.N; i++) {
        cout << data.initialNodes[i].name << " " << data.initialNodes[i].x << " " << data.initialNodes[i].y << endl;
    }
    cout << endl << "# EDGES" << endl;
    for (int i = 0; i < data.E; i++) {
        cout << data.initialNodes[data.initialEdges[i].from].name << " " << data.initialNodes[data.initialEdges[i].to].name << " " << data.initialEdges[i].cost << endl;
    }
    cout << endl << "# FAILED CONNECTION" << endl;
    cout << data.initialNodes[data.failedEdge.from].name << " " << data.initialNodes[data.failedEdge.to].name << endl;
    cout << endl << "# NEW NODE" << endl;
    cout << data.newNode.name << " " << data.newNode.x << " " << data.newNode.y << endl;
    int newEdgecount = data.newEdges.size();
    cout << endl << "# NEW EDGES" << endl << newEdgecount << endl;
    for (int i= 0; i < newEdgecount; i++) {
        cout << data.initialNodes[data.newEdges[i].from].name << " " << data.newNode.name << " " << data.newEdges[i].cost << endl;
    }
    return true;
}

/* Description: Displays a prompt message to the user and reads a full line
-              of text from standard input.
- Parameters: text - string message printed on screen before reading the input
- Return Value: string (the line of text entered by the user)
- Complexity: O(l) l = length of the line read
 */
string readText(string text) {
    cout << text;
    string input;
    getline(cin, input);
    return input;
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
    /* Original if: readInputFile(fileName, data) && buildCandidateGraph(candidates, data)
    Reduced to just readInputFile(fileName, data) for testing */
    if (readInputFile(fileName, data)) {
        // runSimulation(candidates, data);
    } else {
        cout << "No se puede continuar porque el archivo de entrada tiene errores." << endl;
        exitCode = 1;
    }
    return exitCode;
}
