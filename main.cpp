/*OOP Concepts:
        1)Abstract base class
        2)Inheritance
        3)Polymorphism
        4)Template
        5)Friend class and friend function
        6)Operator Overloading
        7)File management system
        8)Constructor destructor
        9)STL
*/
#include<iostream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<fstream>
#include<string>
#include<algorithm>
#include<sstream>

using namespace std;

const int EASY_ROWS = 4;
const int EASY_COLS = 4;
const int EASY_BOMBS = 4;
const int MEDIUM_ROWS = 6;
const int MEDIUM_COLS = 6;//defined constants for difficulty levels
const int MEDIUM_BOMBS = 10;
const int HARD_ROWS = 8;
const int HARD_COLS = 8;
const int HARD_BOMBS = 20;

class Cell{
public:
    bool isBomb;
    bool isRevealed;
    Cell(){
        isBomb= false;
        isRevealed= false;
    }
};
//this is my abstract class
class MinesweeperGrid{
public:
    virtual void display()= 0;
    virtual void reveal(int row,int col)= 0;
    virtual bool isGameOver()= 0;
    virtual ~MinesweeperGrid(){} //destructor
};

template<typename T>
class Grid{
private:
    int numRows;
    int numCols;
    vector<vector<T>>grid;

public:
    Grid(int rows,int cols){
        numRows= rows;
        numCols= cols;
        grid.resize(numRows,vector<T>(numCols));
    }

    T &operator()(int row,int col){
        return grid[row][col];//non-constant version of function call operator overloading in grid class. I can modify elements of the grid through it.it returns a reference to the element at that position, so i can read and write its value.
    }
    const T &operator()(int row, int col) const{
        return grid[row][col];//constant version of function call operator overloading in grid class. It is used when I want to access elements of the grid without modifying them. It returns a constant reference, indicating that i can only read the value of the element but not modify it.
    }

    int getNumRows(){
        return numRows;
    }
    int getNumCols(){
        return numCols;
    }
    friend class Minesweeper;
};

//heart of my minesweeper game implementation.
class Minesweeper:public MinesweeperGrid{
private:
    Grid<Cell> gameGrid; //this member variable is an instance of the Grid template class of type cell
    int numBombs;
    bool gameOver;
    int revealedCount;
    int attempts;
    string playerName;

public:
    Minesweeper(int rows, int cols, int bombs, const string &name)
        : gameGrid(rows, cols), numBombs(bombs), gameOver(false), revealedCount(0), attempts(0), playerName(name) {
        initializeGameGrid();
    }
    void initializeGameGrid() {
        int i,j;
        srand(static_cast<unsigned>(time(nullptr)));
        for (i = 0; i < gameGrid.getNumRows(); ++i){
            for (j = 0; j < gameGrid.getNumCols(); ++j){
                gameGrid(i, j) = Cell();//this function is responsible for initializing the game grid.it randomly places bombs in the grid and sets up the initial state of each cell.
            }
        }
        int bombsPlaced= 0;
        while(bombsPlaced < numBombs){
            int row= rand() % gameGrid.getNumRows();
            int col= rand() % gameGrid.getNumCols();
            if(!gameGrid(row, col).isBomb){
                gameGrid(row, col).isBomb= true;
                ++bombsPlaced;
}}}
    int getNumRows(){
        return gameGrid.getNumRows();
    }
    int getNumCols(){
        return gameGrid.getNumCols();
    }
    void display(){
        int i, j;
        for(i=0;i<gameGrid.getNumRows();++i){
            for(j=0;j<gameGrid.getNumCols();++j){
                if(gameGrid(i,j).isRevealed){
                    if(gameGrid(i,j).isBomb){
                        cout << "* ";//this function is used to display the current state of the Minesweeper grid.
                    }
                    else{
                        int bombCount=countAdjacentBombs(i,j);
                        cout << bombCount << " ";
                    }
                }
                else{
                    cout << "- ";
                }
            }
            cout << endl;
}}

    void reveal(int row,int col){
        int i,j;
        if((row < 0) || (row >= gameGrid.getNumRows()) || (col < 0) || (col >= gameGrid.getNumCols()) || (gameGrid(row, col).isRevealed) || (isGameOver())){
            return;
        }

        if(!gameOver && !gameGrid(row, col).isRevealed){
            ++attempts;//this function allows the player to reveal a cell by specifying its row and column
        }
        gameGrid(row,col).isRevealed=true;

        if(gameGrid(row,col).isBomb){
            gameOver = true;
        }
        else{
            int bombCount=countAdjacentBombs(row,col);
            if (bombCount==0) {
                //will automatically reveal adjacent cells if no bombs nearby
                for(i=-1;i<=1;++i){
                    for (j=-1;j<=1;++j){
                        int newRow=row+i;
                        int newCol=col+j;
                        if((newRow>=0) && (newRow<gameGrid.getNumRows()) && (newCol>=0) && (newCol < gameGrid.getNumCols())){
                            reveal(newRow,newCol);
                        }
                    }
}}}
        ++revealedCount; //will increment revealedCount for each revealed cell
    }

    bool isGameOver(){
        return gameOver || revealedCount == (gameGrid.getNumRows() * gameGrid.getNumCols() - numBombs);
    } //this function checks if the game is over,either because the player hit a bomb or successfully revealed all safe cells

    int countAdjacentBombs(int row,int col){
        int i, j;
        int count=0;
        for(i = -1; i <= 1; ++i){
            for(j=-1;j<= 1;++j){
                int newRow=row+i;
                int newCol=col+j;
                if((newRow>=0) && (newRow<gameGrid.getNumRows()) && (newCol>=0) && (newCol<gameGrid.getNumCols()) && (gameGrid(newRow, newCol).isBomb)){
                    ++count;
                } //this function calculates and returns the count of adjacent bombs for a given cell.
}}
        return count;
}

    void saveGameResultsToFile(const string &difficulty, const string &leaderboardFilename) {
        ofstream outFile(leaderboardFilename, ios::app);
        if(!outFile.is_open()){
            cerr << "Failed to open the leaderboard file for difficulty: " << difficulty << endl;
            return;
        } //This function saves the player's game results, including player name, revealed cell count
        outFile<<"Player: "<<playerName<<" | Cells Unveiled: "<<revealedCount<<" | Attempts: "<<attempts<< endl;
        outFile.close();
        cout<<"Game results for "<<difficulty<<" difficulty saved successfully."<<endl;
}

    void playGame(){//this function allows the player to interact with the game grid, reveal cells, and play the minesweeper game
        while(!isGameOver()){
            display();
            int row,col;
            cout<<"Enter row and column to reveal (for example: 0 0): ";
            cin>>row>>col;
            if((row < 0) || (row >= gameGrid.getNumRows()) || (col < 0) || (col >= gameGrid.getNumCols())){
                cout<<"Invalid input. Please try again."<<endl;
                continue;
            }
            if(!gameGrid(row,col).isRevealed){
                reveal(row,col);
            }
            else{
                cout<<"This cell is already revealed. Choose another one."<<endl;
            }
}
        if(isGameOver()){
            if(gameOver){
                cout<<"Game over! You hit a bomb."<<endl;
            }
            else{
                cout<<"Congratulations! You've won the game."<<endl;
            }
        }
}
friend void displayHelp();
friend void displayMinesweeperHistory();
};

//function to view the leaderboard for a specific difficulty level
void viewLeaderboard(const string &difficulty, const string &leaderboardFilename){
    ifstream inFile(leaderboardFilename);
    if(!inFile.is_open()){
        cerr<<"Failed to open the leaderboard file for difficulty: "<<difficulty<<endl;
        return;
}

    cout<<"Leaderboard for "<<difficulty<<" difficulty (sorted by cells unveiled):"<<endl;

    //created a vector of pairs to store player names and their unveiled cell counts
    vector<pair<string, int>> leaderboard;
    string line;
    while(getline(inFile,line)){
        //parsed the line to extract player name and cells unveiled count
        size_t pos =line.find("Cells Unveiled: ");
        if(pos != string::npos){
            string playerName=line.substr(8, pos - 9);  //it will extract player name
            int cellsUnveiled=stoi(line.substr(pos + 16)); //it will extract cells unveiled count
            leaderboard.push_back(make_pair(playerName, cellsUnveiled));
        }
}

    //this function will sort the leaderboard vector based on cells unveiled count(High to low)
    sort(leaderboard.begin(), leaderboard.end(), [](const pair<string, int> &a, const pair<string, int> &b){
        return (a.second > b.second);
    });

    //will display the sorted leaderboard
    for(const auto &entry : leaderboard) {
        cout<<"Player: "<<entry.first<<" | Cells Unveiled: "<<entry.second<<endl;
}
    inFile.close();
}


// this function is implemented to display instructions on how to play the game
void displayHelp(){
    cout<<"How to Play:"<<endl;
    cout<<"1. You are presented with a grid of cells."<<endl;
    cout<<"2. Your goal is to unveil all cells that do not contain a bomb."<<endl;
    cout<<"3. You can select a cell by specifying its row and column."<<endl;
    cout<<"4. If you select a cell containing a bomb, you lose the game."<<endl;
    cout<<"5. If you select a cell without a bomb, it will display the number of neighboring cells containing bombs."<<endl;
    cout<<"6. Use this information to strategically select the next cell to unveil."<<endl;
    cout<<"7. Keep unveiling cells until you have unveiled all safe cells or hit a bomb."<<endl;
    cout<<"8. You win the game when all safe cells are unveiled, otherwise, you lose."<<endl;
    cout<<"9. Good luck and enjoy the game!"<<endl;
}

//this function is implemented to display information about the history of Minesweeper
void displayMinesweeperHistory(){
    cout<<"History of Minesweeper:"<<endl;
    cout<<"Minesweeper is a classic puzzle video game that was first released in 1990."<<endl;
    cout<<"It was created by Curt Johnson and has since become one of the most popular and iconic puzzle games of all time."<<endl;
    cout<<"The game is known for its simple yet challenging gameplay, where players must use logic and deduction to avoid hidden mines."<<endl;
    cout<<"Minesweeper has been included with every version of Microsoft Windows since Windows 3.1 and has gained a dedicated fanbase worldwide."<<endl;
    cout<<"Its popularity has also led to various adaptations and versions on different platforms."<<endl;
    cout<<"Enjoy playing Minesweeper and exploring its rich history!"<<endl;
}

void startGame(){
    cout<<"Choose an option:"<<endl;
    cout<<"1. Start a new game"<<endl;
    cout<<"2. View Leaderboard"<<endl;
    cout<<"3. Help"<<endl;
    cout<<"4. Learn about Minesweeper History"<<endl;
    cout<<"5. Quit"<<endl;
    int option;
    cin >> option;
    if(option == 1){
        string playerName;
        cout<<"Enter your name: ";
        cin.ignore();
        getline(cin,playerName);

        cout<<"Choose a difficulty level:"<<endl;
        cout<<"1. Easy (4x4 grid with 4 bombs)"<<endl;
        cout<<"2. Medium (6x6 grid with 10 bombs)"<<endl;
        cout<<"3. Hard (8x8 grid with 20 bombs)"<<endl;

        int difficultyOption;
        cin>>difficultyOption;

        int numRows,numCols,numBombs;
        if(difficultyOption == 1) {
            numRows = EASY_ROWS;
            numCols = EASY_COLS;
            numBombs = EASY_BOMBS;
        }
        else if(difficultyOption == 2){
            numRows = MEDIUM_ROWS;
            numCols = MEDIUM_COLS;
            numBombs = MEDIUM_BOMBS;
        }
        else if(difficultyOption == 3){
            numRows = HARD_ROWS;
            numCols = HARD_COLS;
            numBombs = HARD_BOMBS;
        }
        else{
            cout<<"Invalid difficulty level. Starting a new game with default settings."<<endl;
            numRows = EASY_ROWS;
            numCols = EASY_COLS;
            numBombs = EASY_BOMBS;
        }

        Minesweeper game(numRows,numCols,numBombs,playerName);
        game.playGame();

        // will save game results to the appropriate leaderboard file based on difficulty
        if((game.getNumRows() == EASY_ROWS) && (game.getNumCols() == EASY_COLS)){
            game.saveGameResultsToFile("Easy", "easy_leaderboard.txt");
        }
        else if((game.getNumRows() == MEDIUM_ROWS) && (game.getNumCols() == MEDIUM_COLS)){
            game.saveGameResultsToFile("Medium", "medium_leaderboard.txt");
        }
        else if((game.getNumRows() == HARD_ROWS) && (game.getNumCols() == HARD_COLS)){
            game.saveGameResultsToFile("Hard", "hard_leaderboard.txt");
        }
    }
    else if(option == 2){
        cout<<"Choose a difficulty level to view the leaderboard:"<<endl;
        cout<<"1. Easy"<<endl;
        cout<<"2. Medium"<<endl;
        cout<<"3. Hard"<<endl;

        int difficultyOption;
        cin>>difficultyOption;

        if(difficultyOption == 1){
            viewLeaderboard("Easy", "easy_leaderboard.txt");
        }
        else if(difficultyOption == 2){
            viewLeaderboard("Medium", "medium_leaderboard.txt");
        }
        else if(difficultyOption == 3){
            viewLeaderboard("Hard", "hard_leaderboard.txt");
        }
        else{
            cout << "Invalid difficulty level." << endl;
        }
    }
    else if(option == 3){
        displayHelp();
    }
    else if(option == 4){
        displayMinesweeperHistory();
    }
    else if(option == 5){
        cout << "Goodbye!" << endl;
        return;
    }
    else{
        cout << "Invalid option. Please choose a valid option." << endl;
    }
}

int main()
{
    cout<<"----------Welcome to Samioul's Minesweeper Game!----------"<<endl;
    while(1){
        startGame();
        cout<<"Do you want to go back to the main menu? (yes/no): ";
        string playAnother;
        cin>>playAnother;
        if(playAnother != "yes"){
            cout<<"Goodbye!"<<endl;
            break;
        }
    }
    return 0;
}
