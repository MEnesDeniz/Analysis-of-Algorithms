/*
Muhammed Enes Deniz  070170450
Please compile my code as following:
g++ -std=c++11 -Wall -Werror homework3.cpp -o hw3
*/
#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;

// This class is created to represent the elements of the adjancency matrix that is provided by the input file
class Node
{
public:
  int weightOfEdge;   // Weight associated with it
  double probability; // The probability of drop of this edge
  int ID;             // ID associated with this calculated as following row * NodeCount + column
};

// This is a class that I created to store final results and print them accordingly
class Result
{
public:
  int maxFlow;        // Maximum Flow of the subset
  double probability; // Probabaility of subset
};

// This class contains the flow graphs. Also contains method to calculate subset and other essential operations of subsets.
// And finally it contains the main algorithm Ford Fulkerson
class Graph
{
public:
  int numOfNodes;          // Number of all units given in the input file
  int dropPossibleCounter; // Count how many edge could be possibly dropped
  int noOfSubSetDrops;     // Calculate the subset possibility of how many alternative graphs could be created. it is simply 2^(dropPossibleCounter)

  vector<vector<Node>> mainGraph;     // Main Flow Graph
  vector<vector<Node>> residualGraph; // Residual Graph

  vector<Node> dropGraphs;           // Contains which edges will be dropped and store them to ease to access them.
  vector<int> decimalsBinaryEncoded; // To represent subsets binary encoding had enforced, this vector contains the binary representations
  vector<Result> resultToPrint;      // Stores the final results to print

  int sourceAndSink[2] = {0}; // First Index = Source(s) , Second Index = Sink(t)

  unordered_map<int, vector<int>> dropMap;         // Contains the possible drops units(edges-nodes) ID's.
  unordered_map<int, double> probabilityMap;       // Contains the possible drops probability
  unordered_map<int, double> flowToProbabilityMap; // Maps the calculated flows to their probabilities.
  unordered_map<int, vector<int>> pathCreator;     // Contains the valid paths

  queue<int> queue_BFS; // Queue used by BFS

  void dropEdge(int);               // Drops the given ID's weights on residual graph
  void subSetCalculator();          // Calculate the subsets based on how many drop possibility 2^(dropPossibleCounter)
  void maxFlowCalculator();         // Calculate the maxFlow of the given graph by using fordFulkerson
  void binaryToProbabilityMapper(); // Calculate the each subset dropping indexes and probabilities.

  void flowToProbabilityMapper(int, int); // Map the calculated flow to corresponding probability
  double acquireProbability(int);         // Access the specific unit(Node) probability

  void sortAndPrint(); // Sort the final result by probability and print it out respectively.

  int BFS(vector<int> &, int &, int &); // Find the valid path from source the sink and if there exists valid path calculate the possible maximum flow of it.
  int fordFulkerson();                  // This function returns the maximum flow from source(s) the sink(t)
};

// Transform the giving decimal to its binary representation
int binaryEncoding(int incomingDecimal)
{
  int reminder = 1;
  int scaler = 1;
  int calculatedBinary = 0;
  // Fundamental operation to transform decimal to binary.
  while (incomingDecimal != 0)
  {
    reminder = incomingDecimal % 2;
    incomingDecimal = incomingDecimal / 2;
    calculatedBinary = calculatedBinary + (reminder * scaler);
    scaler = scaler * 10;
  }
  return calculatedBinary;
}

// Find the valid path from s to t and if there exists valid path calculate the minimum residual capacity along the edges and return 200 as success.
// If there exists not a valid path return 201 as failure

int Graph::BFS(vector<int> &listOfparent, int &flowOfPath, int &pathTrack)
{
  int pathElement;
  int parentOfPathElement;
  pathTrack = 0;

  queue_BFS.push(sourceAndSink[0]);    // First all of push the source to queue to start with it.
  listOfparent[sourceAndSink[0]] = -1; // Source does not have parent so indicate a terminal value with it.

  // Create a boolean array and fill it with false.
  bool visited[numOfNodes];
  for (int i = 0; i < numOfNodes; i++)
    visited[i] = false;

  visited[sourceAndSink[0]] = true; // Indicate that you are on the source node and you have visited it
  vector<int> indexList;            // Create an indexList to store the path's each element

  // BFS loop run it until the queue becomes empty
  while (!queue_BFS.empty())
  {
    // Pop up the value and operate on it.
    int examineNode = queue_BFS.front();
    queue_BFS.pop();

    // Look the poped item's each neighboor whether they have any edge between them and operate respectively on the edge's weight.
    for (int currentOne = 0; currentOne < numOfNodes; currentOne++)
    {
      // Look for the specific edge and acquire it's weight.
      int capacityOfEdge = residualGraph[examineNode][currentOne].weightOfEdge;

      if (visited[currentOne] == false && capacityOfEdge > 0) // The vertice should not be visited and associated edge's weight shoul be greater than 0
      {
        if (currentOne == sourceAndSink[1]) // If you come all along to sink node the valid path exists.
        {

          listOfparent[currentOne] = examineNode;
          flowOfPath = 1001; // maximum W(e) indicated in the homework pdf

          pathElement = sourceAndSink[1]; // Start from the sink and reversely go the path
          // Calculate the minimum residual capacity of the path
          while (pathElement != sourceAndSink[0])
          {
            parentOfPathElement = listOfparent[pathElement];

            // Store the path indexes in an unordered map to later update the capacities of the edges.
            indexList.push_back(parentOfPathElement);
            indexList.push_back(pathElement);
            pathCreator[pathTrack] = indexList;

            // Finding the minimum residual capacity or maximum flow of the path.
            flowOfPath = min(flowOfPath, residualGraph[parentOfPathElement][pathElement].weightOfEdge);

            // Reset and update the necessary variables that is used in the loop for finding the minimum residual capacity.
            indexList.clear();
            pathElement = listOfparent[pathElement];
            pathTrack++;
          }

          return 200;
        }

        // Traverse in the graph by the BFS algorithm mentality.
        queue_BFS.push(currentOne);
        listOfparent[currentOne] = examineNode;
        visited[currentOne] = true;
      }
    }
  }

  return 201;
}

// Finding and returning the maximum flow of the given graph in the indicated source to sink.
int Graph::fordFulkerson()
{
  int firstIndex = 0;
  int secondIndex = 0;
  int maxFlow = 0;
  int flowOfPath = 0;

  int pathTrack = 0;                    // Track how many elements exists in the given path.
  vector<int> listOfparent(numOfNodes); // Helper vector to find the path.

  // Augmenting operation done in here, simply if there exists a valid path find by the BFS done this operation repetadely.
  while ((BFS(listOfparent, flowOfPath, pathTrack) == 200))
  {
    // Residual capacities are updated by following the path reversely.
    for (int i = 0; i < pathTrack; i++)
    {
      firstIndex = pathCreator[i][0];
      secondIndex = pathCreator[i][1];
      residualGraph[firstIndex][secondIndex].weightOfEdge -= flowOfPath;
      residualGraph[secondIndex][firstIndex].weightOfEdge += flowOfPath;
    }
    maxFlow += flowOfPath; // the update maximum flow with path flow.
  }

  return maxFlow;
}

// If the corresponding edge needs to be drooped make it weight as 0 to indicated it is dropped.
void Graph::dropEdge(int ID)
{
  int row = 0;
  int column = 0;

  // Find the place of the edge from it's ID by reversing the operation row * NodeCount + column
  if (ID == -1) // IF ID = -1 this means do not drop any index terminal value is sended
  {
    return;
  }

  if (ID >= numOfNodes) // ID greater than numOfNodes means that the edge is not in the first row
  {
    row = ID / numOfNodes;
    column = ID % numOfNodes;
  }
  else // this edge must be placed in the first row
  {
    row = 0;
    column = ID % numOfNodes;
  }

  residualGraph[row][column].weightOfEdge = 0; // DROP
}

// Calculate the subset possibility of how many alternative graphs could be created
void Graph::subSetCalculator()
{
  noOfSubSetDrops = pow(2, dropPossibleCounter); // Subset number is simply 2^(dropPossibleCounter)
  int encodedValue = 0;
  // Encode the decimal values of to binary to represent each subset
  // Later on by looking the bit's we will indicate which to drop
  for (int i = 0; i < noOfSubSetDrops; i++)
  {
    encodedValue = binaryEncoding(i);
    decimalsBinaryEncoded.push_back(encodedValue);
  }
}

// Acquire the respective unit's probability by calculating it's index values from the ID
double Graph::acquireProbability(int ID)
{
  int row = 0;
  int column = 0;
  if (ID >= numOfNodes)
  {
    row = ID / numOfNodes;
    column = ID % numOfNodes;
  }
  else
  {
    row = 0;
    column = ID % numOfNodes;
  }

  return mainGraph[row][column].probability;
}

// Using the binary representation we have already find the possible number of subsets.
// Now map the which binary representation stand which ones to drop and their probability.
void Graph::binaryToProbabilityMapper()
{
  vector<int> dropList;
  int toDrop = 0;
  int mapElementCount = 0;
  int correspondingBit = 0;
  double probability = 1.0;
  int acquiredID = 0;

  while (mapElementCount < noOfSubSetDrops) // Iterate through until you have covered all of the subset.
  {

    toDrop = decimalsBinaryEncoded[mapElementCount]; // Acquire the binary representation of the subset.
    // Need to acquire each bit of the binary representation it have bit number equals to the possible number of dropped units.
    for (int j = 0; j < dropPossibleCounter; j++)
    {
      // If the corresponding bit has value 1 it means drop so needs to indicate the dropped unit(look it into the droppable units vector)
      correspondingBit = toDrop % 10;
      acquiredID = dropGraphs[j].ID;
      if (correspondingBit == 1) // If you drop it it's probability is equal to the drop probability
      {
        dropList.push_back(acquiredID);
        probability = probability * acquireProbability(acquiredID);
      }
      else
      { // If you dont drop it, it's probability is 1 - (probability of it to be dropped.)
        probability = probability * (1.0 - acquireProbability(acquiredID));
      }
      toDrop = toDrop / 10; // Now go for the next bit.
    }

    // Store the subsets calculated probability.
    probabilityMap[mapElementCount] = probability;

    if (dropList.empty()) // If no element dropped just indicate it with terminal value -1
    {
      dropList.push_back(-1);
      dropMap[mapElementCount] = dropList;
    }
    else // Store the subsets dropped ID's
    {
      dropMap[mapElementCount] = dropList;
    }

    // Reset and update the variables that is used in the loop
    probability = 1.0;
    mapElementCount++;
    dropList.clear();
  }
}

// Map the flow values to the correspind probability
void Graph::flowToProbabilityMapper(int maxFlowRecent, int index)
{
  if (flowToProbabilityMap.empty()) // If no element exists just initate it
  {
    flowToProbabilityMap[maxFlowRecent] = probabilityMap[index];
  }
  else
  {
    auto it = flowToProbabilityMap.find(maxFlowRecent);

    // If the corresponding flow exists in the container then dont create new entry just update it probability.
    if (it != flowToProbabilityMap.end())
      it->second += probabilityMap[index];
    else // If there is not a maximum flow like that then place it and map the probability.
      flowToProbabilityMap[maxFlowRecent] = probabilityMap[index];
  }
}

// Calculate the max flow for the each subset flow graph.
void Graph::maxFlowCalculator()
{
  binaryToProbabilityMapper(); // Find the subsets crucial information (dropped ID's vector, probabilities associated with subsets)

  int mapElementCount = 0;
  int maxFlow = 0;
  while (mapElementCount < noOfSubSetDrops) // Calculate each subsets maximum flow
  {
    residualGraph = mainGraph;                                     // Reset the residual graph as mainGraph
    for (int i = 0; i < int(dropMap[mapElementCount].size()); i++) // Drop the edge indicated in the drooped ID's vector
    {
      dropEdge(dropMap[mapElementCount][i]);
    }

    maxFlow = fordFulkerson();                         // Calculate the Maximum flow
    flowToProbabilityMapper(maxFlow, mapElementCount); // Map the maximum flow with it's probability.

    mapElementCount++;
  }
  sortAndPrint(); // Print the results as according to indicated in PDF
}

// I could not managed to sort the unordered map as to my desires, so I have found a remedy solution
// Store them a user defined vector then sort it according to the custom function.
void Graph::sortAndPrint()
{
  Result tempResult;
  for (auto x : flowToProbabilityMap)
  {
    tempResult.maxFlow = x.first;
    tempResult.probability = x.second;
    resultToPrint.push_back(tempResult);
  }

  sort(resultToPrint.begin(), resultToPrint.end(), [](const Result &element1, const Result &element2)
       { return element1.probability > element2.probability; });

  double total = 0.0;
  for (int i = 0; i < int(resultToPrint.size()); i++)
  {
    total = total + (resultToPrint[i].probability * resultToPrint[i].maxFlow); // The calculation indicated in the PDF had been carried out.
    cout << "Probability of occurence : " << resultToPrint[i].probability << ", "
         << "Maximum Flow: " << resultToPrint[i].maxFlow << endl;
  }
  cout << "\n"
       << "Expected Maximum Flow from node " << sourceAndSink[0] + 1 << " to node " << sourceAndSink[1] + 1 << ": " << total << endl;
}

void readFromfile(Graph &myGraph, const string file_name);

int main(int argc, char *argv[])
{
  Graph myGraph;

  const string file_name = argv[1];
  int source = atoi(argv[2]);
  int sink = atoi(argv[3]);

  myGraph.sourceAndSink[0] = source - 1;
  myGraph.sourceAndSink[1] = sink - 1;

  readFromfile(myGraph, file_name);
  myGraph.subSetCalculator();
  myGraph.maxFlowCalculator();

  return 0;
}

// Read From File
void readFromfile(Graph &myGraph, const string file_name)
{
  ifstream file;
  file.open(file_name);

  int weight;
  double probability;
  myGraph.dropPossibleCounter = 0;
  if (!file.is_open())
  {
    cout << "Reading file cannot be opened!" << endl;
  }
  else
  {
    file >> myGraph.numOfNodes;
    vector<Node> tempV;
    Node *tempN = new Node(); // Fill the mainGraph and residualGraph
    for (int i = 0; i < myGraph.numOfNodes; i++)
    {
      for (int j = 0; j < myGraph.numOfNodes; j++)
      {
        file >> weight;
        tempN->weightOfEdge = weight;
        tempN->ID = i * myGraph.numOfNodes + j;
        tempV.push_back(*tempN);
      }
      myGraph.mainGraph.push_back(tempV);
      myGraph.residualGraph.push_back(tempV);
      tempV.clear();
    }

    delete tempN; // Delete the temporary object to prevent memory leak

    for (int i = 0; i < myGraph.numOfNodes; i++) // Update the unit's with the given probabilities correspondingly.
    {
      for (int j = 0; j < myGraph.numOfNodes; j++)
      {
        file >> probability;
        myGraph.mainGraph[i][j].probability = probability;
        if (probability > 0)
        {
          myGraph.dropGraphs.push_back(myGraph.mainGraph[i][j]);
          myGraph.dropPossibleCounter++;
        }
      }
    }
  }
}
