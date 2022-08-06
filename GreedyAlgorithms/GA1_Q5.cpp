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
    Node();
    vector<Node *> neighbors; // List of Neighbors
};

// Node Constructor
Node::Node()
{
    this->ID = 0;
}

// Graph Class
class SNAGraph
{
private:
    int vertices;

public:
    vector<Node> Graph;          // Main Graph that Keeps the Node
    int VerticesColor[35] = {0}; // Container to keep every node's color

    SNAGraph(int);
    void addNeighbor(int, int); // Add the neighbor nodes to each other's neighborlist
    int validColoring(int);     // Function to check whether the given color is valid or not
    int GA1Coloring();          // Graph Coloring Function following presented Greedy Algorithm 2

    int getVerticesNo() { return this->vertices; };
};

SNAGraph::SNAGraph(int vertices)
{
    this->vertices = vertices;
}

// If one of the neighbor is already colored with that color it is not possible to take that color
int SNAGraph::validColoring(int coloredIndex)
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

int SNAGraph::GA1Coloring()
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
void SNAGraph::addNeighbor(int hostNeighbor, int correspondingNeighbor)
{
    Graph[hostNeighbor].neighbors.push_back(&Graph[correspondingNeighbor]);

    Graph[correspondingNeighbor].neighbors.push_back(&Graph[hostNeighbor]);
}

void addRelationsToGraph(SNAGraph &SNA)
{
    Node *tempN = new Node();
    for (int i = 0; i < SNA.getVerticesNo(); i++)
    {
        tempN->ID = i;
        SNA.Graph.push_back(*tempN);
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

void printResults(SNAGraph &SNA, int numberOfDifferentColors)
{
    for (int index = 0; index < SNA.getVerticesNo(); index++) // prints vertex colors
        cout << "Vertex " << index + 1 << " ---> Color " << SNA.VerticesColor[index] << endl;
    cout << endl;
    SNA.Graph.clear();
    cout << "Number of different colors: " << numberOfDifferentColors << endl;
}

int main()
{
    auto program_start = chrono::steady_clock::now(); // Start the clock when the program starts
    SNAGraph SNA(35);
    addRelationsToGraph(SNA);
    int numberOfDifferentColors = SNA.GA1Coloring();

    auto program_end = chrono::steady_clock::now(); // End the clock when the program in finish step.

    printResults(SNA, numberOfDifferentColors);
    cout << "The running time: " << float(chrono::duration_cast<chrono::microseconds>(program_end - program_start).count()) / 1000 << " ms" << endl;
    return 0;
}