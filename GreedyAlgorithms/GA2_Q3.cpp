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
    vector<Node *> neighbors; // List of Neighbors
};

// Node Constructor
Node::Node()
{
    this->ID = 0;
    this->degree = 0;
}

class RegionsGraph
{
private:
    int vertices;

public:
    vector<Node> Graph;               // Main Graph that Keeps the Node
    vector<int> descendingDegreeList; // Container to keep the Node's ID in terms of descending degree
    vector<int> visitedVertices;      // Container to keep visited vertices
    int VerticesColor[7] = {0};       // Container to keep every node's color

    RegionsGraph(int);
    void addNeighbor(int, int);                 // Add the neighbor nodes to each other's neighborlist
    void degreeDescending();                    // Obtain the sorted order of descending degree
    int validColoring(int);                     // Function to check whether the given color is valid or not
    int GA2Coloring();                          // Graph Coloring Function following presented Greedy Algorithm 2
    bool NeighborChecker(int);                  // Check whether the given Node is neighbor of the already colored Node
    void DegreeListAdjuster(int &);             // After the coloring operation drop the visited vertices from the descendingDegreeList
    void NeighborVerticesToBeColored(int, int); // Color the node's with highest degree node color.

    int getVertexNo() { return this->vertices; };
};

RegionsGraph::RegionsGraph(int vertices)
{
    this->vertices = vertices;
}

// Obtaining the vector of descending degree of node's ID.
void RegionsGraph::degreeDescending()
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
int RegionsGraph::validColoring(int coloredIndex)
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
bool RegionsGraph::NeighborChecker(int indexToBeChecked)
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
void RegionsGraph::DegreeListAdjuster(int &notVisited)
{
    // Break Into A Function
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
void RegionsGraph::NeighborVerticesToBeColored(int index, int color)
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
        // Still need to be checked again since it may be neighbor with some other vertex
        int validity = validColoring(descendingDegreeList[index]);
        if (validity >= 0)
        {
            cout << "Checking " << descendingDegreeList[index] + 1 << " ---> false (since it is connected to " << validity + 1 << ")" << endl;
            VerticesColor[descendingDegreeList[index]] = 0;
        }
        else
        { // If there is not a neighbor with the same color then you could color your vertice with the current highest degree vertice color.
            cout << "Checking " << descendingDegreeList[index] + 1 << " ---> true" << endl;
            cout << "Vertex " << descendingDegreeList[index] + 1 << " ---> Color " << color << endl;
            visitedVertices.push_back(descendingDegreeList[index]);
        }
    }
}

// Greedy Algorithm 2 For Graph Coloring
int RegionsGraph::GA2Coloring()
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
void RegionsGraph::addNeighbor(int hostNeighbor, int correspondingNeighbor)
{
    Graph[hostNeighbor].neighbors.push_back(&Graph[correspondingNeighbor]);
    Graph[hostNeighbor].degree++;

    Graph[correspondingNeighbor].neighbors.push_back(&Graph[hostNeighbor]);
    Graph[correspondingNeighbor].degree++;
}

void addRelationsToGraph(RegionsGraph &TurkeyRegions)
{
    Node *tempN = new Node();
    for (int i = 0; i < TurkeyRegions.getVertexNo(); i++)
    {
        tempN->ID = i;
        TurkeyRegions.Graph.push_back(*tempN);
        TurkeyRegions.descendingDegreeList.push_back(0);
    }
    delete tempN;
    // Marmara Region
    TurkeyRegions.addNeighbor(0, 1);
    TurkeyRegions.addNeighbor(0, 3);
    TurkeyRegions.addNeighbor(0, 4);

    // Black Sea Region
    // TurkeyRegions.addNeighbor(1, 0);
    TurkeyRegions.addNeighbor(1, 2);
    TurkeyRegions.addNeighbor(1, 3);

    // Eastern Anatolia
    // TurkeyRegions.addNeighbor(2,1);
    TurkeyRegions.addNeighbor(2, 3);
    TurkeyRegions.addNeighbor(2, 5);
    TurkeyRegions.addNeighbor(2, 6);

    // Central Anatolia
    // TurkeyRegions.addNeighbor(3,0);
    // TurkeyRegions.addNeighbor(3,1);
    // TurkeyRegions.addNeighbor(3,2);
    TurkeyRegions.addNeighbor(3, 4);
    TurkeyRegions.addNeighbor(3, 5);

    // Aegean
    // TurkeyRegions.addNeighbor(4,0);
    // TurkeyRegions.addNeighbor(4,3);
    TurkeyRegions.addNeighbor(4, 5);

    // Mediterranean
    // TurkeyRegions.addNeighbor(5,2);
    // TurkeyRegions.addNeighbor(5,3);
    // TurkeyRegions.addNeighbor(5,4);
    TurkeyRegions.addNeighbor(5, 6);

    // Southeast Anatolia
    // TurkeyRegions.addNeighbor(6,2);
    // TurkeyRegions.addNeighbor(6,5);

    /*for (int i = 0; i < TurkeyRegions.getVertexNo(); i++)
    {
        cout << TurkeyRegions.Graph[i].degree;
        cout << "h" << endl;
    }*/
}

void printResults(RegionsGraph &TurkeyRegions, int mincolor)
{
    TurkeyRegions.Graph.clear();
    cout << "Well done!! All the vertices are colored." << endl;
    cout << "Min color num:" << mincolor << endl;
}
int main()
{
    RegionsGraph TurkeyRegions(7);
    addRelationsToGraph(TurkeyRegions);

    auto program_start = chrono::steady_clock::now(); // Start the clock when the program starts
    TurkeyRegions.degreeDescending();
    int mincolor = TurkeyRegions.GA2Coloring();
    auto program_end = chrono::steady_clock::now(); // End the clock when the program in finish step.
    
    printResults(TurkeyRegions, mincolor);

    cout << "The running time: " << float(chrono::duration_cast<chrono::microseconds>(program_end - program_start).count()) / 1000 << " ms" << endl;
    return 0;
}