/*Muhammed Enes Deniz 070170450*/

#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <bits/stdc++.h>
using namespace std;

// Node Class
class Node
{
public:
    int ID;
    int degree;
    Node();
    vector<Node *> neighbors; // List of Adjacent Neighbors
};

// Node Constructor
Node::Node()
{
    this->ID = 0;
    this->degree = 0;
}

// Graph Class
class SNAGraph
{
private:
    int vertices;

public:
    vector<Node> Graph;               // Main Graph that Keeps the Node
    vector<int> descendingDegreeList; // Container to keep the Node's ID in terms of descending degree
    vector<int> visitedVertices;      // Container to keep visited vertices
    int VerticesColor[35] = {0};      // Container to keep every node's color

    SNAGraph(int);
    void addNeighbor(int, int);                 // Add the neighbor nodes to each other's neighborlist
    void degreeDescending();                    // Obtain the sorted order of descending degree
    int validColoring(int);                     // Function to check whether the given color is valid or not
    int GA2Coloring();                          // Graph Coloring Function following presented Greedy Algorithm 2
    bool NeighborChecker(int);                  // Check whether the given Node is neighbor of the already colored Node
    void DegreeListAdjuster(int &);             // After the coloring operation drop the visited vertices from the descendingDegreeList
    void NeighborVerticesToBeColored(int, int); // Color the node's with highest degree node color.

    int getVerticesNo() { return this->vertices; }; // Getter for Vertices
};

// Constructor for SNAGraph
SNAGraph::SNAGraph(int vertices)
{
    this->vertices = vertices;
}

// Obtaining the vector of descending degree of node's ID.
void SNAGraph::degreeDescending()
{
    // Sort the Degree's and place the ID's according to it.
    int maxDegreePlacer = 0;
    int verticesNumber = vertices;
    while (verticesNumber) // While not the all vertices checked keep loop
    {
        int maximumDegreeNumber = 0;
        int verticesTracker = 0;
        while (verticesTracker < vertices) // Do not look outside of the graph
        {
            if (Graph[verticesTracker].degree > maximumDegreeNumber) // If the Current Vertice Degree is greater than the Max Degree
            {
                maximumDegreeNumber = Graph[verticesTracker].degree;     // Make it as new max degree
                descendingDegreeList[maxDegreePlacer] = verticesTracker; // Place it's ID to the appropriate place in the descendingDegreeList
            }
            verticesTracker++;
        }
        Graph[descendingDegreeList[maxDegreePlacer]].degree = 0; // Make the degree of the node as 0 to avoid that it will not checked again
        maxDegreePlacer++;                                       // Increase the index location of the descendingDegreeList
        verticesNumber--;
    }
}

// If there exists a neighbor node that colored with the same retake it.
int SNAGraph::validColoring(int coloredIndex)
{
    int neighborSize = int(Graph[coloredIndex].neighbors.size()); // Neighbors of the recently colored node
    int neighbor = 0;
    while (neighbor < neighborSize) // Look for all the neighbors
    {
        if (VerticesColor[coloredIndex] == VerticesColor[Graph[coloredIndex].neighbors[neighbor]->ID]) // If there exists a neighbor with the same color
        {
            return Graph[coloredIndex].neighbors[neighbor]->ID; // Return that neighbors ID, to indicate which one is causing that
        }

        neighbor++;
    }
    return -38; // If there exists not a neighbor with same color return a sentinel value which cannot be a node ID.
}

// If the Node is immediate neighbor of the current node with the higgest Edge(descendingdegreeList[0]) then it is not appropriate to take color in this turn.
bool SNAGraph::NeighborChecker(int indexToBeChecked)
{
    int size = Graph[descendingDegreeList[0]].neighbors.size();
    for (int i = 0; i < size; i++)
    { // If it is the neighbor of the current node with the higgest edge it cannot be colored in this turn/round/coloring order.
        if (Graph[descendingDegreeList[0]].neighbors[i]->ID == descendingDegreeList[indexToBeChecked])
        {
            return true;
        }
    }
    return false;
}

// After the coloring ends for this round/turn clean up the visited vertices
void SNAGraph::DegreeListAdjuster(int &notVisited)
{
    if (!visitedVertices.empty())
    {
        int counter = 0;
        int sizeOfVisited = visitedVertices.size();
        cout << "Vertices ";
        while (!visitedVertices.empty())
        {
            int indexOfVisitedVertice = visitedVertices[0] + 1;
            if (sizeOfVisited == 1) // If there exists only one colored/visited index just named it
            {
                cout << indexOfVisitedVertice;
            }
            else
            { // This conditions stand for the appropriate printing. If you're printing the last dropped one dont put comma in the end of it.
                if (counter == sizeOfVisited - 1)
                {
                    cout << indexOfVisitedVertice;
                }
                else
                {
                    cout << indexOfVisitedVertice << ",";
                }
            }

            visitedVertices.erase(visitedVertices.begin()); // Erase/Drop the visited vertices as depicted in the greedy algorithm

            // Also make sure that you have erased/dropped them in the descendingDegreeList so they will not be checked again.
            std::vector<int>::iterator iter;
            iter = find(descendingDegreeList.begin(), descendingDegreeList.end(), indexOfVisitedVertice - 1);
            if (iter != descendingDegreeList.end())
            {
                descendingDegreeList.erase(iter);
                notVisited--;
            }
            counter++;
        }
    }
}

// After coloring the highest degree vertice go for and look whether any possible vertice could you colored with the same color
void SNAGraph::NeighborVerticesToBeColored(int index, int color)
{
    if (NeighborChecker(index)) // If you are trying to color a vertice that is neighbor with the current highest edge it wont color
    {
        cout << "Checking " << descendingDegreeList[index] + 1 << " ---> false" << endl;
    }
    else
    {
        // It may be not neighbor of the current highest degree vertice so you could color it with it same color of highest degree but you should check still
        // whether this vertice's have any other neighbor with the same color if it's then you should revert the coloring operation.
        VerticesColor[descendingDegreeList[index]] = color;
        int validity = validColoring(descendingDegreeList[index]);
        if (validity >= 0)
        {
            cout << "Checking " << descendingDegreeList[index] + 1 << " ---> false (since it is connected to " << validity + 1 << ")" << endl;
            VerticesColor[descendingDegreeList[index]] = 0;
        }
        else // If there is not a neighbor with the same color then you could color your vertice with the current highest degree vertice color.
        {
            cout << "Checking " << descendingDegreeList[index] + 1 << " ---> true" << endl;
            cout << "Vertex " << descendingDegreeList[index] + 1 << " ---> Color " << color << endl;
            visitedVertices.push_back(descendingDegreeList[index]);
        }
    }
}

// Greedy Algorithm 2 For Graph Coloring
int SNAGraph::GA2Coloring()
{
    int notVisitedVerticesNumber = vertices;
    int color = 1;
    while (notVisitedVerticesNumber) // Loop until all the vertices are visited
    {
        // Color the highest degree vertice with the current color

        visitedVertices.push_back(descendingDegreeList[0]);
        VerticesColor[descendingDegreeList[0]] = color;
        cout << "Vertex " << descendingDegreeList[0] + 1 << " ---> Color " << color << endl;

        // Try to find whether a vertice could be colored with the same color of the highest degree vertice
        for (int i = 1; i < notVisitedVerticesNumber; i++)
        {
            NeighborVerticesToBeColored(i, color);
        }

        // Drop the visited vertices
        DegreeListAdjuster(notVisitedVerticesNumber);
        cout << " are dropped!!\n"
             << endl;

        // If there exists still vertices to be colored than increase the color by 1
        if (notVisitedVerticesNumber != 0)
        {
            color++;
        }
    }

    return color;
}

// Add the neighborship(adjancency) relationship between the nodes to their neighborlist
void SNAGraph::addNeighbor(int hostNeighbor, int correspondingNeighbor)
{
    Graph[hostNeighbor].neighbors.push_back(&Graph[correspondingNeighbor]);
    Graph[hostNeighbor].degree++;

    Graph[correspondingNeighbor].neighbors.push_back(&Graph[hostNeighbor]);
    Graph[correspondingNeighbor].degree++;
}

void addRelationsToGraph(SNAGraph &SNA)
{
    Node *tempN = new Node();
    for (int i = 0; i < SNA.getVerticesNo(); i++)
    {
        tempN->ID = i;
        SNA.Graph.push_back(*tempN);
        SNA.descendingDegreeList.push_back(0);
    }
    delete tempN;
    SNA.addNeighbor(0, 1);
    SNA.addNeighbor(0, 6);

    SNA.addNeighbor(1, 2);
    SNA.addNeighbor(1, 5);

    SNA.addNeighbor(2, 3);

    SNA.addNeighbor(3, 4);
    SNA.addNeighbor(3, 10);
    SNA.addNeighbor(3, 11);

    SNA.addNeighbor(4, 5);

    SNA.addNeighbor(5, 6);
    SNA.addNeighbor(5, 9);

    SNA.addNeighbor(6, 7);
    SNA.addNeighbor(6, 8);

    SNA.addNeighbor(7, 8);
    SNA.addNeighbor(7, 20);

    SNA.addNeighbor(8, 9);
    SNA.addNeighbor(8, 19);

    SNA.addNeighbor(9, 10);
    SNA.addNeighbor(9, 17);
    SNA.addNeighbor(9, 19);

    SNA.addNeighbor(10, 11);
    SNA.addNeighbor(10, 16);

    SNA.addNeighbor(11, 12);

    SNA.addNeighbor(12, 13);
    SNA.addNeighbor(12, 15);

    SNA.addNeighbor(13, 14);

    SNA.addNeighbor(14, 15);
    SNA.addNeighbor(14, 26);

    SNA.addNeighbor(15, 16);
    SNA.addNeighbor(15, 26);

    SNA.addNeighbor(16, 17);
    SNA.addNeighbor(16, 24);
    SNA.addNeighbor(16, 25);

    SNA.addNeighbor(17, 18);
    SNA.addNeighbor(17, 31);

    SNA.addNeighbor(18, 22);
    SNA.addNeighbor(18, 23);

    SNA.addNeighbor(19, 20);
    SNA.addNeighbor(19, 21);

    SNA.addNeighbor(20, 21);

    SNA.addNeighbor(21, 22);
    SNA.addNeighbor(21, 33);
    SNA.addNeighbor(21, 34);

    SNA.addNeighbor(22, 23);
    SNA.addNeighbor(22, 32);

    SNA.addNeighbor(23, 31);
    SNA.addNeighbor(23, 32);

    SNA.addNeighbor(24, 25);
    SNA.addNeighbor(24, 29);

    SNA.addNeighbor(25, 26);
    SNA.addNeighbor(25, 29);

    SNA.addNeighbor(26, 27);
    SNA.addNeighbor(26, 29);

    SNA.addNeighbor(27, 28);

    SNA.addNeighbor(28, 29);
    SNA.addNeighbor(28, 30);

    SNA.addNeighbor(29, 30);
    SNA.addNeighbor(29, 31);

    SNA.addNeighbor(30, 31);

    SNA.addNeighbor(31, 32);

    SNA.addNeighbor(32, 33);

    SNA.addNeighbor(33, 34);
}

void printResults(SNAGraph &SNA, int mincolor)
{
    SNA.Graph.clear();
    cout << "Well done!! All the vertices are colored." << endl;
    cout << "Min color num:" << mincolor << endl;
}
int main()
{
    SNAGraph SNA(35);
    addRelationsToGraph(SNA);

    auto program_start = chrono::steady_clock::now(); // Start the clock when the program starts
    SNA.degreeDescending();
    int mincolor = SNA.GA2Coloring();
    auto program_end = chrono::steady_clock::now(); // End the clock when the program in finish step.

    printResults(SNA, mincolor);
    
    cout << "The running time: " << float(chrono::duration_cast<chrono::microseconds>(program_end - program_start).count()) / 1000 << " ms" << endl;
    return 0;
}