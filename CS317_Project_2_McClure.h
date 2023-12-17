// Purpose: Header file for CS 317 Project 2
// Author: Colby McClure
// This file contains all of the logic for running Prim's algorithm on a given set of nodes 

// *************************************************************** // 
// Below are the libraries used in the program
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector> 
#include <cmath>
using namespace std;
// *************************************************************** //

// This global variable is used to store the number of nodes in the graph, will be used later for the number of iterations in the for loop
int num; 

// This node structure stores the x and the y value of each node
struct Node {
    int x;
    int y;
    Node(int x, int y): x(x), y(y) {}
}; 

// *************************************************************** //
// This function calculates the euclidean distance between two nodes
double euclideanDistance(Node u, Node v) {
    return sqrt(pow((u.x - v.x), 2) + pow((u.y - v.y), 2)); 
}

// Initialize the vector of all of the total nodes 
vector<Node> nodes;

// This edge structure will store the two nodes along with their weight that gets calculated in the euclideanDistance function
struct Edge {
    Node u;
    Node v;
    double weight;
    Edge(Node u, Node v): u(u), v(v), weight(euclideanDistance(u, v)) {}
};

// Initialize the vector of all of the total edges
vector<Edge> edges;
// *************************************************************** //

// The MinHeap structure will be used to store the edges in a min heap
struct MinHeap {

    // Initializes the heap vector that will be used in the below methods
    vector<Edge> heap;

    // This method will be used to heapify the heap vector, its argument will be the index of the vector
    void heapify(int i) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        // This function will sort the edges by their respective weights in the heap
        if (left < heap.size() && heap[left].weight < heap[smallest].weight) {
            smallest = left;
        }

        if (right < heap.size() && heap[right].weight < heap[smallest].weight) {
            smallest = right;
        }

        if (smallest != i) {
            swap(heap[i], heap[smallest]);
            heapify(smallest);
        }
    }

    // This method will be used to delete an edge from the heap
    void deleteEdge(Edge e) {
        for (int i = 0; i < heap.size(); i++) {
            if (heap[i].u.x == e.u.x && heap[i].u.y == e.u.y && heap[i].v.x == e.v.x && heap[i].v.y == e.v.y) {
                heap[i] = heap.back();
                heap.pop_back();
                heapify(i);
                break;
            }
        }
    }

    // This method will be used to extract the minimum edge and deletes the minimum edge from the heap
    Edge extractMin() {
        Edge min = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapify(0);
        return min;
    }

    // This method will be used to insert an edge into the heap 
    void insert(Edge e) {
        heap.push_back(e);
        int i = heap.size() - 1;
        int parent = (i - 1) / 2;

        while (i > 0 && heap[parent].weight > heap[i].weight) {
            swap(heap[i], heap[parent]);
            i = parent;
            parent = (i - 1) / 2;
        }
    }

}; 
// *************************************************************** //

// This function will be used to output the results to the output.txt file using the MST vector that is passed in as an argument
void outputFile(vector<Edge> MST) {
    ofstream out;
    out.open("output.txt", ios::out);
    
    // This loop will print out how the edges connect as well as the weight of each edge to the output.txt file
    for (int i = 0; i < MST.size(); i++) {
        out << "(" << MST[i].u.x << ", " << MST[i].u.y << ") - (" << MST[i].v.x << ", " << MST[i].v.y << "): " << MST[i].weight << endl;
    }
    out.close();
}

// *************************************************************** //
// This function is where the Prim's algorithm is implemented. It uses the nodes and edges vectors as arguments
void primMST(vector<Node> nodes, vector<Edge> edges) {

    // This vector will store the edges that are in the MST
    vector<Edge> MST;

    // This vector is for the nodes that have already been visited
    vector<Node> visited;

    // This variable is used to store the number of nodes in the graph
    const int num = nodes.size();

    // Start by pushing back the very first node into the visited vector, since it will be the starting node of the MST
    visited.push_back(nodes[0]);

    // Initialize the min heap
    MinHeap heap; 

    // Initialize the total weight of the MST that will be used later to print out the total weight of the MST
    double totalWeight = 0;

    // To start the algorithm off, we need to insert all of the edges that are connected to the starting node into the heap
    // For every edge that is connected to the starting node, insert it into the heap
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i].u.x == visited.back().x && edges[i].u.y == visited.back().y) {
            heap.insert(edges[i]);
        }
    }

    // This while loop will run until all of the nodes have been visited
    while(visited.size() < nodes.size()) {

        // Initialize the minimum edge to be the minimum edge in the heap 
        Edge min_edge = heap.extractMin();

        // The first conditional in the loop checks for edges in the edges vector that contain the minimum's initial node as the destination node for all of the edges. If so, then delete it
        // The second conditional in the loop checks for edges in the edges vector that contain the minimum's destination node as the destination node for all of the edges. If so, then delete it
        // Since a node can only be visited once, these conditionals will delete any unecessary edges from both the edges vector and the min heap  
        // It will delete both elements in from the heap and the edges vector
        for(int i = 0; i < edges.size(); i++) {
            if(edges[i].v.x == min_edge.u.x && edges[i].v.y == min_edge.u.y) {
                heap.deleteEdge(edges[i]);
                edges.erase(edges.begin() + i);
            }
            if(edges[i].v.x == min_edge.v.x && edges[i].v.y == min_edge.v.y) {
                heap.deleteEdge(edges[i]);
                edges.erase(edges.begin() + i);
            }
        }
        
        // After everything has been deleted, then the min heap gets reloaded with new values based on what minimum edges destination was
        for(int i = 0; i < edges.size(); i++) {
            if(min_edge.v.x == edges[i].u.x && min_edge.v.y == edges[i].u.y) {
                heap.insert(edges[i]);
            }
        }

        // The minimum edge's destination gets pushed back into the visited nodes vector 
        visited.push_back(min_edge.v);

        // The minimum edge gets pushed back into the MST vector
        MST.push_back(min_edge);
    }

    // This will count of the total weight of all of the edges that were used and store them into a variable to be printed
    for(int i = 0; i < MST.size(); i++) {
        totalWeight += MST[i].weight;
    }
    cout << "The total weight is: " << totalWeight << endl;

    // This function call will output the results to the output.txt file
    outputFile(MST);
} 
// *************************************************************** //

// This function is primarily for reading the input file and building the graph using nodes and edges vectors
void readAndBuild(string input) {

    // Initialize the input file and the lines that will be read in
    fstream in;
    string line; 

    // Initialize the x and y values that will be read in from the input file to be stored into the nodes vector
    int x, y;  

    // Open the input file
    in.open(input, ios::in); 

    // Initialize the nodes vector
    vector<Node> nodes;

    // If the input file cannot be opened, then print out an error message and return
    if(!in.is_open()) {
        cout << "Error opening file." << endl; 
        return; 
    }
    
    // This will read in the first line of the input file and store it into the num variable
    getline(in, line); 
    stringstream ss(line);
    ss >> num; 

    // This loop will read in the rest of the lines in the input file and store them into the nodes vector
    while (getline(in, line)) {
        stringstream ss(line);
        
        // This skips the whitespace in the input file
        ss >> ws;  

        // This will ignore the first "(" character in the input file
        ss.ignore(1); 

        // Input the x variable 
        ss >> x;

        // This will ignore the comma and the space in the input file
        ss.ignore(2); 

        // Input the y variable
        ss >> y;

        // Create a new node and push it back into the nodes vector
        Node node(x, y);
        nodes.push_back(node);
    } 
    in.close();
    
    // This for loop will be what connects all of the nodes together to make it a complete graph
    for (int i = 0; i < nodes.size(); i++) {
        for (int j = 0; j < nodes.size(); j++) {
            if (i != j) {
                Edge edge(nodes[i], nodes[j]);
                edges.push_back(edge);
            }
        }
    }

    // Makes the function call to run Prim's algorithm
    primMST(nodes, edges);
} 


