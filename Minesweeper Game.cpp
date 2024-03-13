#include <iostream>
#include <ctime>
#include <vector>

using namespace std;

// Function to clear the console
void clear()
{
	printf("\33[2J\33[1;1H");
}

// Function to set the difficulty level of the game
void Difficulty(int &n, int &side, int &mines, int &secside, int &maxmoves) {
    cout << "\t\t\t\t\t\t\tWelcome to Minesweeper! \n\t\t\t\t\t\tChoose a difficulty level: \n\t\t\t\t\t\t1. Beginner (9x9 grid with 10 mines) \n\t\t\t\t\t\t2. Intermediate (16x16 grid with 40 mines) \n\t\t\t\t\t\t3. Expert (16x30 grid with 99 mines) \n\t\t\t\t\t\tEnter your choice: ";
    cin >> n;
    if (n == 1) {
        side = secside = 9;
        mines = 10;
    } else if (n == 2) {
        side = secside = 16;
        mines = 40;
    } else if (n == 3) {
        side = 16;
        secside = 30;
        mines = 99;
    } else {
        cout<<"Please choose number between 1 and 3 :)"<<endl;
        Difficulty(n, side, mines, secside, maxmoves);
    }
    maxmoves = side * secside - mines;
}

// Function to check if the placement of the mine is valid
bool mineValidPlacement(vector<vector<int>>& realboard, int x, int y) {
    return realboard[x][y] != 10;
}

// Function to check if the given cell is valid in the context of the board
bool isValid(int row, int column, int side, int secside) {
    return row >= 0 && row < side && column >= 0 && column < secside;
}

// Function to place mines on the board randomly
void PlaceMines(vector<vector<int>>& board, vector<vector<int>>& realboard, int mines, int side, int secside) {
    srand(time(0));
    for (int i = 0; i < mines; ++i) {
        int x, y;
        do {
            x = rand() % side;
            y = rand() % secside;
        } while (!mineValidPlacement(realboard, x, y));
        realboard[x][y] = 10; // 10 represents a mine
    }
}

// Function to print the current state of the board
void PrintBoard(const vector<vector<int>>& board, int side, int secside, int mines, int flagsPlaced, clock_t startTime) {
    int minesRemaining = mines - flagsPlaced;
    clock_t currentTime = clock();
    float timeElapsed = (float)(currentTime - startTime) / CLOCKS_PER_SEC;
    cout << "\tMines remaining: " << minesRemaining << " | Time elapsed: " << timeElapsed << "s" << endl;
    cout << "  ";
    for (int i = 1; i <= secside; i++)
    {   
        if(i<10) {
        cout << "  " << i << " ";
        } else {
            cout << " " << i << " ";
        }
    }
    cout << endl;
    for(int i=0; i<side; i++)
    {
        if(i<9) {
            cout << i + 1 << "  ";
        } else {
            cout << i + 1 << " ";
        }

        for(int j=0; j<secside; j++)
        {
            if (board[i][j] == 11) cout << "|-| ";
            else if (board[i][j] == 10) cout << "|*| ";
            else if (board[i][j] == 12) cout << "|F| ";
            else cout << "|" << board[i][j] << "| ";
        }
        cout<<endl;
    }
}

void EndBoard(const vector<vector<int>>& board, vector<vector<int>>& realboard, int side, int secside) {
    cout << "  ";
    for (int i = 1; i <= secside; i++)
    {   
        if(i<10) {
        cout << "  " << i << " ";
        } else {
            cout << " " << i << " ";
        }
    }
    cout << endl;
    for(int i=0; i<side; i++)
    {
        if(i<9) {
            cout << i + 1 << "  ";
        } else {
            cout << i + 1 << " ";
        }

        for(int j=0; j<secside; j++)
        {
            if (board[i][j] == 11 && realboard[i][j] != 10) cout << "|-| ";
            else if (board[i][j] == 12) cout << "|F| ";
            else if (realboard[i][j] == 10) cout << "|*| ";
            else cout << "|" << board[i][j] << "| ";
        }
        cout<<endl;
    }    
}

// Function to recursively uncover adjacent cells
void UncoverAdjacentCells(int row, int column, vector<vector<int>>& board, vector<vector<int>>& realboard, int side, int secside, int& maxmoves) {
    if (!isValid(row, column, side, secside) || board[row][column] != 11) return;

    board[row][column] = realboard[row][column];
    --maxmoves;

    if (realboard[row][column] != 0) return; // Stop if the cell is not a 0

    // Directions to uncover: up, down, left, right, and diagonals
    int directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    for (int i = 0; i < 8; ++i) {
        int newRow = row + directions[i][0];
        int newColumn = column + directions[i][1];
        UncoverAdjacentCells(newRow, newColumn, board, realboard, side, secside, maxmoves);
    }
}

// Function to calculate the numbers for the board
void CalculateNumbers(vector<vector<int>>& realboard, int side, int secside) {
    for (int row = 0; row < side; ++row) {
        for (int column = 0; column < secside; ++column) {
            if (realboard[row][column] == 10) continue; // Skip mines
            int mineCount = 0;
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    int nr = row + dx, nc = column + dy;
                    if (isValid(nr, nc, side, secside) && realboard[nr][nc] == 10) ++mineCount;
                }
            }
            realboard[row][column] = mineCount;
        }
    }
}


int main() {
    int n, side, secside, mines, maxmoves, flagsPlaced = 0;
    string a;
    clock_t startTime = clock();
    clear();
    Difficulty(n, side, mines, secside, maxmoves);

    vector<vector<int>> board(side, vector<int>(secside, 11)); // 11 represents an unopened cell
    vector<vector<int>> realboard(side, vector<int>(secside, 0)); // Initially, all cells are 0

    PlaceMines(board, realboard, mines, side, secside);
    CalculateNumbers(realboard, side, secside);

    while (maxmoves > 0) {
        clear();
        PrintBoard(board, side, secside, mines, flagsPlaced, startTime);
        
        int row, column;
        char command;
        if(n==1) cout << "\t\tCommands:\n\t- Uncover Cell: u row col\n\t- Flag Cell: f row col\n\t- Delete the flag: d row col\n\t  Enter your command: ";
        else if(n==2) cout << "\t\t\t\tCommands:\n\t\t\t- Uncover Cell: u row col\n\t\t\t- Flag Cell: f row col\n\t\t\t- Delete the flag: d row col\n\t\t\t  Enter your command: ";
        else if(n==3) cout << "\t\t\t\t\t\t\tCommands:\n\t\t\t\t\t\t- Uncover Cell: u row col\n\t\t\t\t\t\t- Flag Cell: f row col\n\t\t\t\t\t\t- Delete the flag: d row col\n\t\t\t\t\t\t  Enter your command: ";
        cin >> command >> row >> column;
        --row; --column; // Adjust for 0-indexed board

        if (command == 'u') {
            if (!isValid(row, column, side, secside)) continue;
            if (realboard[row][column] == 10) {
                clear();
                cout << "Game Over! You hit a mine!" << endl;
                EndBoard(board, realboard, side, secside);
                cout<<"Do you want to play one more game (y/n)? "<<endl;
                cin>>a;
                if(a=="y")
                {
                    clear();
                    main();
                } else {
                    cout<<"Thanks for game!"<<endl;
                    break;
                }
            } else {
                UncoverAdjacentCells(row, column, board, realboard, side, secside, maxmoves);
            }
        } else if (command == 'f') {
            if (isValid(row, column, side, secside) && board[row][column] == 11) { // Ensure the cell isn't already flagged or uncovered
                board[row][column] = 12; // Flag the cell
                flagsPlaced++;
            }
        } else if (command == 'd') {
            if (isValid(row, column, side, secside) && board[row][column] == 12) { // Ensure the cell is flagged
                board[row][column] = 11; // Remove flag
                flagsPlaced--;
            }
        } else {
            cout<<"Enter your command one more time: "<<endl;
        }

        if (maxmoves == 0) {
            clear();
            PrintBoard(board, side, secside, mines, flagsPlaced, startTime);
            cout << "Congratulations! You cleared all non-mine cells!" << endl;
            cout<<"Do you want to play one more game (y/n)? "<<endl;
            cin>>a;
            if(a=="y")
            {
                clear();
                main();
            } else {
                cout<<"Thanks for game!"<<endl;
                break;
            }
        }
    }
    return 0;
}