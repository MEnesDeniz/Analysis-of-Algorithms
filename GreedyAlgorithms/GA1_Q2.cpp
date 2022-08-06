/*Muhammed Enes Deniz 070170450*/

#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <bits/stdc++.h>
using namespace std;

class Node
{
public:
    int ID;
    Node();
    vector<Node *> neighbors; // List of Neighbors
};

// Node Constructor
Node::Node()
{
    this->ID = 0;
}

// Graph Class
class RegionsGraph
{
private:
    int vertices;

public:
    vector<Node> Graph;
    int VerticesColor[7] = {0};

    RegionsGraph(int);
    void addNeighbor(int, int); // Add the neighbor nodes to each other's neighborlist
    int validColoring(int);     // Function to check whether the given color is valid or not
    int GA1Coloring();          // Graph Coloring Function following presented Greedy Algorithm 2
    int getVertexNo() { return this->vertices; };
};

RegionsGraph::RegionsGraph(int vertices)
{
    this->vertices = vertices;
}

// If one of the neighbor is already colored with that color it is not possible to take that color
int RegionsGraph::validColoring(int coloredIndex)
{
    int NeighborToBeChecked;
    int neighborSize = Graph[coloredIndex].neighbors.size();
    int neighbor = 0;
    // Look for the current colored vertices neighbors
    while (neighbor < neighborSize)
    {
        // If one of the neighbor is colored with same color you cannot color this vertice with that color
        NeighborToBeChecked = Graph[coloredIndex].neighbors[neighbor]->ID;
        if (VerticesColor[NeighborToBeChecked] == VerticesColor[coloredIndex])
            return -38;

        neighbor++;
    }
    return 1;
}

int RegionsGraph::GA1Coloring()
{
    int differentColors = 0;
    // Look for the all vertices
    for (int vertexToColor = 0; vertexToColor < vertices; vertexToColor++)
    {
        // Also check the all vertices neighbor to whether the given color is valid or not
        for (int colorNumber = 1; colorNumber <= vertices; colorNumber++)
        {
            // Look for the highest color number which indicates how many different colors used
            if (colorNumber > differentColors)
                differentColors = colorNumber;

            VerticesColor[vertexToColor] = colorNumber;
            if (validColoring(vertexToColor) == -38)
            {
                // Retake the color since it is not valid
                VerticesColor[vertexToColor] = 0;
                continue;
            }
            else
            {
                break;
            }
        }
    }

    return differentColors;
}

// Add the neighborship(adjancency) relationship between the nodes to their neighborlist
void RegionsGraph::addNeighbor(int hostNeighbor, int correspondingNeighbor)
{
    Graph[hostNeighbor].neighbors.push_back(&Graph[correspondingNeighbor]);

    Graph[correspondingNeighbor].neighbors.push_back(&Graph[hostNeighbor]);
}

void addRelationsToGraph(RegionsGraph &TurkeyRegions)
{
    Node *tempN = new Node();
    for (int i = 0; i < TurkeyRegions.getVertexNo(); i++)
    {
        tempN->ID = i;
        TurkeyRegions.Graph.push_back(*tempN);
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

void printResults(RegionsGraph &TurkeyRegions, int numberOfDifferentColors)
{
    for (int index = 0; index < TurkeyRegions.getVertexNo(); index++) // prints vertex colors
        cout << "Vertex " << index + 1 << " ---> Color " << TurkeyRegions.VerticesColor[index] << endl;
    cout << endl;
    TurkeyRegions.Graph.clear();
    cout << "Number of different colors: " << numberOfDifferentColors << endl;
}
int main()
{
    auto program_start = chrono::steady_clock::now(); // Start the clock when the program starts
    RegionsGraph TurkeyRegions(7);
    addRelationsToGraph(TurkeyRegions);
    int numberOfDifferentColors = TurkeyRegions.GA1Coloring();
    auto program_end = chrono::steady_clock::now(); // End the clock when the program in finish step.

    printResults(TurkeyRegions, numberOfDifferentColors);
    cout << "The running time: " << float(chrono::duration_cast<chrono::microseconds>(program_end - program_start).count()) / 1000 << " ms" << endl;
    return 0;
}
