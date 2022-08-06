//Muhammed Enes Deniz 070170450
#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <stack>
#include <list>
#include <string>
#include <algorithm>

#include <chrono>
#include <unistd.h>

using namespace std;

int RC = 0;

// Since I declared my attribitues as private I have to write getter and setter functions.
// Defining attribitues in public is not often practiced for security purposes thus I followed the notions that I learned in BLG252E

// Node Class
class Node
{
private:
    int ID;
    int shipAvaiable;
    bool Visited;
    bool isPushed;

public:
    Node(int = 0);             // Constructor of Node Class
    vector<Node *> neighBours; // List of Neighbors

    // Setters for Node
    void set_ShipAvaiable(int ship) { this->shipAvaiable = ship; };
    void set_ID(int id) { this->ID = id; };
    void set_Visited(bool flag) { this->Visited = flag; };
    void set_IsPushed(bool flag) { this->isPushed = flag; };

    // Getters for Node
    bool get_Visited() { return this->Visited; };
    bool get_IsPushed() { return this->isPushed; };
    int get_ShipAvaiable() { return this->shipAvaiable; };
    int get_ID() { return this->ID; };
};

// Constructor of Node Class
Node::Node(int ID)
{
    this->ID = ID;
    this->Visited = false;
    this->isPushed = false;
    this->shipAvaiable = 0;
}

// Gameboard Class
class GameBoard
{
private:
    int x_Attack;
    int y_Attack;
    int shipNodesCount;
    string gametype;
    int PlayedMoveCount;
    int NodesCreated;
    string playerName;

public:
    // Setters for GameBoard
    void set_xAttack(int x_attack) { this->x_Attack = x_attack; };
    void set_yAttack(int y_attack) { this->y_Attack = y_attack; };
    void set_ShipNodesNo(int ship_count) { this->shipNodesCount = ship_count; };
    void set_Gametype(string gametype) { this->gametype = gametype; };
    void set_PlayedMoveCount(int move) { this->PlayedMoveCount = move; };
    void set_NodesCreated(int created) { this->NodesCreated = created; };
    void set_PlayerName(string name) { this->playerName = name; };

    // Getters for GameBoard
    string get_PlayerName() { return this->playerName; };
    int get_NodesCreated() { return this->NodesCreated; };
    int get_xAttack() { return this->x_Attack; };
    int get_yAttack() { return this->y_Attack; };
    int get_ShipNodes() { return this->shipNodesCount; };
    int get_PlayedMoveCount() { return this->PlayedMoveCount; };
    string get_GameType() { return this->gametype; };

    // 2D Vector to store Node's
    vector<vector<Node>> board;

    // Stack that will use for DFS movements
    stack<Node *> stack_dfs;

    // Queue that will use for BFS movements
    queue<Node *> queue_bfs;

    bool DFS(GameBoard &); // DFS Algo
    bool BFS(GameBoard &); // BFS Algo
};

// DFS Algorithm
bool GameBoard::DFS(GameBoard &competitor)
{
    //As accordance to my calculations of edge cases this while loop will be never taken
        //But If a potentially not covered an edge case this loop will make sure that algorithm works in right order
    while (stack_dfs.top()->get_Visited()) // Check whether the the top of the stack node is not visited and until it's not visited pop
    {
        stack_dfs.pop();
    }

    // Store and pop the top of the DFS stack which will be our attack location
    Node *strikedNode = stack_dfs.top();
    stack_dfs.pop();
    // Since attack location is decided increase the movecount
    PlayedMoveCount++;
    // Make sure that this node is setted as visited
    strikedNode->set_Visited(true);

    if (strikedNode->get_ShipAvaiable() == 1) // Check whether the enemy has a ship in the attack location
    {
        strikedNode->set_ShipAvaiable(0);                           // Attack the location and damage the ship
        competitor.set_ShipNodesNo(competitor.get_ShipNodes() - 1); // Make sure that enemy's fleet is  damaged.

        if (competitor.get_ShipNodes() == 0) // Check whether enemy has any ship left
        {
            return true; // If no enemy ships left indicate that you win the game
        }
    }

    // Push the all non visited neighbors of the current attack location into the stack
    // Neighbor list kept in the priority order thus make sure that you reversely push into the stack to keep the priority order
    for (int i = (int(strikedNode->neighBours.size()) - 1); i >= 0; i--)
    {
        if (!strikedNode->neighBours[i]->get_Visited())
        {
            stack_dfs.push(strikedNode->neighBours[i]); // Push the non visited neighbor to the stack
        }
    }
    return false;
}

// BFS Algorithm
bool GameBoard::BFS(GameBoard &competitor)
{
    // Store and pop the top of the BFS queue which will be our attack location
    Node *strikedNode = queue_bfs.front();
    queue_bfs.pop();
    // Since attack location is decided increase the movecount
    PlayedMoveCount++;
    // Make sure that this node is setted as visited
    strikedNode->set_Visited(true);

    if (strikedNode->get_ShipAvaiable() == 1) // Check whether the enemy has a ship in the attack location
    {
        strikedNode->set_ShipAvaiable(0);                           // Attack the location and damage the ship
        competitor.set_ShipNodesNo(competitor.get_ShipNodes() - 1); // Make sure that enemy's fleet is  damaged.

        if (competitor.get_ShipNodes() == 0) // Check whether enemy has any ship left
        {
            return true; // If no enemy ships left indicate that you win the game
        }
    }

    // Push the all non visited neighbors and non pushed neighbors of the current attack location into the queue
    // Neighbor list kept in the priority order so directly pushing into the queue will keep the priorities of neighboord also in the queue.
    for (int i = 0; i < int(strikedNode->neighBours.size()); i++)
    {
        if (!strikedNode->neighBours[i]->get_Visited() && !strikedNode->neighBours[i]->get_IsPushed())
        {
            strikedNode->neighBours[i]->set_IsPushed(true); // Make sure you indicate that this node is placed into the queue.
            queue_bfs.push(strikedNode->neighBours[i]);     // Push the non visited neighbor to the queue
        }
    }

    return false;
}

// This function will ensure that players will play DFS Battle Ship Game in turn based order
string dfsGame(GameBoard &p1, GameBoard &p2)
{
    Node *move1 = &p2.board[p1.get_xAttack()][p1.get_yAttack()]; // First Attack Location of Player1
    Node *move2 = &p1.board[p2.get_xAttack()][p2.get_yAttack()]; // First Attack Location of Player2
    p1.stack_dfs.push(move1);
    p2.stack_dfs.push(move2);
    string gamewinner;
    bool isFinished = false;
    p1.set_PlayedMoveCount(0);
    p2.set_PlayedMoveCount(0);

    while (!isFinished) // Check whether the game is finished
    {
        isFinished = p1.DFS(p2); // Player1 will attack to the player2 with DFS strategy

        if (isFinished) // Check whether player1 is won the game
        {
            gamewinner = p1.get_PlayerName();
            break;
        }

        isFinished = p2.DFS(p1); // Player2 will attack to the player1 with DFS strategy

        if (isFinished) // Check whether player2 is won the game
        {
            gamewinner = p2.get_PlayerName();
            break;
        }
    }

    // Clear the nodes in the 2D Vectors
    p1.board.clear();
    p2.board.clear();

    return gamewinner;
}

// This function will ensure that players will play BFS Battle Ship Game in turn based order
string bfsGame(GameBoard &p1, GameBoard &p2)
{
    Node *move1 = &p2.board[p1.get_xAttack()][p1.get_yAttack()]; // First Attack Location of Player1
    Node *move2 = &p1.board[p2.get_xAttack()][p2.get_yAttack()]; // First Attack Location of Player2
    p1.queue_bfs.push(move1);
    p2.queue_bfs.push(move2);
    string gamewinner;
    bool isFinished = false;
    p1.set_PlayedMoveCount(0);
    p2.set_PlayedMoveCount(0);

    while (!isFinished) // Check whether the game is finished
    {
        isFinished = p1.BFS(p2); // Player1 will attack to the player2 with BFS strategy

        if (isFinished) // Check whether player1 is won the game
        {
            gamewinner = p1.get_PlayerName();
            break;
        }

        isFinished = p2.BFS(p1); // Player2 will attack to the player1 with BFS strategy

        if (isFinished) // Check whether player2 is won the game
        {
            gamewinner = p2.get_PlayerName();
            break;
        }
    }

    // Clear the nodes in the 2D Vectors
    p1.board.clear();
    p2.board.clear();

    return gamewinner;
}

// Print game details.
void printGameDetails(GameBoard &player1, GameBoard &player2, const string &winner)
{
    cout << "The algorithm: Player1: " << player1.get_GameType() << " Player2: " << player2.get_GameType() << endl;
    cout << "The number of visited nodes : Player1: " << player1.get_PlayedMoveCount() << ","
         << " Player2 : " << player2.get_PlayedMoveCount() << endl;
    cout << "The number of nodes kept in the memory: Player1: " << player1.get_NodesCreated() << ","
         << " Player2: " << player2.get_NodesCreated() << endl;
    cout << "The result: " << winner << " won!" << endl;
}

void readFromfile(GameBoard &playerboard, string file_name);

int main(int argc, char *argv[])
{
    auto program_start = chrono::steady_clock::now(); // Start the clock when the program starts

    if (argc != 3)
    {
        cout << "Missing Arguments!" << endl;
        return 1;
    }

    const string player1_input = argv[1]; // Read the file name for player1
    const string player2_input = argv[2]; // Read the file name for player2
    string winner;

    GameBoard player1board; // Gameboard object for player1
    player1board.set_PlayerName("Player1");
    readFromfile(player1board, player1_input); // Construct the board of the player1 according to player1_input file

    GameBoard player2board; // Gameboard object for player2
    player2board.set_PlayerName("Player2");
    readFromfile(player2board, player2_input); // Construct the board of the player2 according to player2_input file

    // Depending of the algo type choose the gamestyle
    if (player1board.get_GameType() == "BFS" && player2board.get_GameType() == "BFS")
    {
        winner = bfsGame(player1board, player2board);
    }
    else if (player1board.get_GameType() == "DFS" && player2board.get_GameType() == "DFS")
    {
        winner = dfsGame(player1board, player2board);
    }

    printGameDetails(player1board, player2board, winner);
    auto program_end = chrono::steady_clock::now(); // End the clock when the program in finish step.
    cout << "The running time: " << chrono::duration_cast<chrono::milliseconds>(program_end - program_start).count() << "ms" << endl;
    return 0;
}

void readFromfile(GameBoard &playerboard, const string file_name)
{

    ifstream file;
    playerboard.set_ShipNodesNo(0);

    file.open(file_name); // Open the file

    if (!file.is_open())
    {
        cout << "Reading file cannot be opened!" << endl;
    }
    else
    {
        string algo_type;
        int N, ship_number, loc_x_start, loc_x_end, loc_y_start, loc_y_end, x_Attack, y_Attack;
        file >> algo_type >> x_Attack >> y_Attack;
        file >> N >> ship_number;
        RC = N;

        playerboard.set_Gametype(algo_type);
        playerboard.set_xAttack(x_Attack);
        playerboard.set_yAttack(y_Attack);

        vector<Node> tempV;
        Node *tempN = new Node();

        // Prepare each row vector with inner loop
        // Then push this row vector to the 2D vector to  accomplish to acquire 2D Vector in the right order
        int NodeCounter = 0;
        for (int i = 0; i < RC; i++)
        {
            for (int j = 0; j < RC; j++)
            {
                NodeCounter++;
                playerboard.set_NodesCreated(NodeCounter);
                tempN->set_ID((i * RC) + j);
                tempV.push_back(*tempN);
            }
            playerboard.board.push_back(tempV);
            tempV.clear();
        }
        delete tempN; // Delete the temporary object to prevent memory leak

        string shiplocation;
        int temp_ship_number = ship_number;
        // Place the ships to the right locations.
        // The ships could be in varying size but it could be only be vertical or horizontally placed
        for (int i = 0; i < temp_ship_number; i++)
        {
            file >> loc_x_start >> loc_y_start >> loc_x_end >> loc_y_end;

            if (loc_x_start == loc_x_end) // x locations of the given ship coordinate equals so if it's length is greater than 1 it's vertically placed
            {
                int ship_size = loc_y_end - loc_y_start; // Calculate the ship length
                for (int i = 0; i <= ship_size; i++)     // Place the ship parts to the corresponding matrix locations
                {
                    playerboard.board[loc_x_start][loc_y_start + i].set_ShipAvaiable(1);
                    playerboard.set_ShipNodesNo(playerboard.get_ShipNodes() + 1); // Increase the fleet of player
                }
            }
            else if (loc_y_start == loc_y_end) // y locations of the given ship coordinate equals so if it's length is greater than 1 it's horizontally placed
            {
                int ship_size = loc_x_end - loc_x_start; // Calculate the ship length
                for (int i = 0; i <= ship_size; i++)     // Place the ship parts to the corresponding matrix locations
                {
                    playerboard.board[loc_x_start + i][loc_y_start].set_ShipAvaiable(1);
                    playerboard.set_ShipNodesNo(playerboard.get_ShipNodes() + 1); // Increase the fleet of player
                }
            }
        }
        file.close();

        // The neighbors list(Adjancency List) is constructed for every node in the matrix.
        // Priority TOP >> LEFT >> BOT >> RIGHT so placement will be followed this order
        for (int i = 0; i < RC; i++) // ROW
        {
            for (int j = 0; j < RC; j++) // COLUMN
            {
                if (i == 0 && j == 0) // UP LEFT CORNER
                {
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i + 1][j]); // BOT
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i][j + 1]); // RIGHT
                }

                else if (i == 0 && j == RC - 1) // UP RIGHT CORNER
                {
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i][j + -1]); // LEFT
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i + 1][j]);  // BOT
                }
                else if (i == 0 && j != 0 && j != RC - 1) // FIRST ROW EXCLUDED UP LEFT - RIGHT CORNERS
                {
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i][j + -1]); // LEFT
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i + 1][j]);  // BOT
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i][j + 1]);  // RIGHT
                }

                else if (i == RC - 1 && j == 0) // DOWN LEFT CORNER
                {
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i - 1][j]); // TOP
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i][j + 1]); // RIGHT
                }

                else if (i == RC - 1 && j == RC - 1) // DOWN RIGHT CORNER
                {
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i - 1][j]);  // TOP
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i][j + -1]); // LEFT
                }

                else if (i == RC - 1 && j != RC - 1 && j != 0) // LAST ROW EXCLUDED DOWN LEFT - RIGHT CORNERS
                {
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i - 1][j]);  // TOP
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i][j + -1]); // LEFT
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i][j + 1]);  // RIGHT
                }

                else if (j == 0 && i != 0 && i != RC - 1) // FIRST COLUMN EXCLUDED LEFT UP - DOWN CORNERS
                {
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i - 1][j]); // TOP
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i + 1][j]); // BOT
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i][j + 1]); // RIGHT
                }
                else if (j == RC - 1 && i != RC - 1 && i != 0) // LAST COLUMN EXCLUDED RIGHT UP DOWN CORNERS
                {
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i - 1][j]);  // TOP
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i][j + -1]); // LEFT
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i + 1][j]);  // BOT
                }
                else // MIDDLE NODES
                {
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i - 1][j]);  // TOP
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i][j + -1]); // LEFT
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i + 1][j]);  // BOT
                    playerboard.board[i][j].neighBours.push_back(&playerboard.board[i][j + 1]);  // RIGHT
                }
            }
        }
    }
}

/*
void printBoard(GameBoard &opponent)
{
    cout << "Openent State (" << opponent.get_PlayerName() << "):" << endl;
    for (int i = 0; i < RC; i++)
    {
        for (int j = 0; j < RC; j++)
        {
            if (opponent.board[i][j].get_Visited())
            {
                if (j > 0)
                    cout << " x";
                else
                    cout << "x";
            }
            else
            {
                if (j > 0)
                    cout << " .";
                else
                    cout << ".";
            }
        }
        cout << endl;
    }
}
*/
