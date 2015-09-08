#include <iostream>
#include<fstream>
#include <string>
#include <iterator>
#include <algorithm>
#include "grid.h"
#include "lifeutil.h"

using namespace std;

const char ALIVE = 'X';
const char DEAD = '-';
const int ANIMATE_TICK_COUNT = 30;
const int ANIMATE_TICK_PAUSE = 100;

/*
* Function: loadGrid
* Usage: loadGrid(fileName, result(out))
* ---------------------------------------
* Will load the given grid with data from a file.
* Throws <code>invalid_argument</code> if file cannot be found.
* Note: This function makes assumptions regarding file format and does not define
* behaviour for non-standard formats.
*/
void loadGrid(const string fileName, Grid<char>& result){

    //Open file stream.
    ifstream fileStream {fileName};

    //If opening the file stream failed we return nullptr.
    if (!fileStream)
    {
       throw invalid_argument("Error: Could not find/open file " + fileName + "\n");
    }

    int width;
    int height;

    //Read grid width and height.
    fileStream >> height;
    fileStream >> width;

    //Resize the grid to the gived width and height.
    result.resize(height, width);
    //Copy relevant elements from stream to grid.
    copy_n(istream_iterator<char>{fileStream}, width*height, begin(result));

    //Close file stream. (Closes automatically when ifstream goes out of scope?)
    fileStream.close();
}

/*
* Function: countNeighboursAlive
* Usage: int count = countNeighboursAlive(grid, cellY, cellX)
* ---------------------------------------
* Returns the number of cells alive directly surrounding the given cell.
*/
int countNeighboursAlive(const Grid<char>& grid, const int cellY, const int cellX){

    //Calculate the loop range for all cells surrounding the given cell.
    int minX = max(0, cellX-1);
    int minY = max(0, cellY-1);
    int maxX = min(grid.nCols - 1, cellX+1);
    int maxY = min(grid.nRows - 1, cellY+1);

    int count = 0;

    //Loop through all cells in the calculated range.
    for(int y = minY; y <= maxY; y++){
        for(int x = minX; x <= maxX; x++){

            //The given cell will be in this range, so we make sure to skip it.
            if(x != cellX || y != cellY){
                if(grid.get(y, x) == ALIVE){
                    count++;
                }
            }
        }
    }

    return count;
}

/*
* Function: update
* Usage: udpate(grid(out))
* ---------------------------------------
* Advances the <code>grid</grid> with one tick.
*/
void update(Grid<char>& grid){

    //Create a placeholder grid and resize it to the size of the original grid.
    Grid<char> temp;
    temp.resize(grid.nRows, grid.nCols);
    int aliveCount;

    for(int y = 0; y < grid.nRows; y++){
        for(int x = 0; x < grid.nCols; x++){

            //Get number of neighbours alive for each cell in the original grid.
            aliveCount = countNeighboursAlive(grid, y, x);


            //Take the appropriate action on the placeholder grid depending on neighbours alive.
            if(aliveCount < 2){

                temp.set(y, x, DEAD);
            }
            else if(aliveCount > 3){

                temp.set(y, x, DEAD);
            }
            else if(aliveCount == 2){

                temp.set(y, x, grid.get(y, x));
            }
            else if(aliveCount == 3){

                temp.set(y, x, ALIVE);
            }
        }
    }

    //Replace our original grid with a copy of the placeholder grid.
    grid = temp;
}


/*
* Function: renderGrid
* Usage: renderGrid(grid)
* ---------------------------------------
* Outputs the grid to the console.
*/
void renderGrid(const Grid<char>& grid){

    //Loops through every cell in grid and outputs it to the screen.
    for(int y = 0; y < grid.nRows; y++){
        for(int x = 0; x < grid.nCols; x++){

            cout << grid.get(y, x);
        }
        cout << endl;
    }
}

/*
* Function: animate
* Usage: udpate(grid(out), tickCount, pauseTime)
* ---------------------------------------
* Advances the <code>grid</grid> by <code>tickCount</code> number of ticks,
* rendering the grid after each tick and pausing for
* <code>pauseTimer</code> milliseconds between each tick.
*/
void animate(Grid<char>& grid, const int tickCount, const int pauseTime){

    for(int i = 0; i < tickCount; i++){

        update(grid);
        pause(pauseTime);
        clearConsole();
        renderGrid(grid);
    }
}

/*
* Function: menuLoop
* Usage: menuLoop(grid(out))
* ---------------------------------------
* Allows the user to choose between three actions.
* Animate: Animates the <code>grid</code> for 30 ticks with 100 miliseconds between each tick.
* Tick: Advance the <code>grid</code> with one tick.
* Quit: Exit the program.
*/
void menuLoop(Grid<char>& grid){

    char input;
    bool run = true;

    while(run){
        cout << "a)nimate, t)ick, q)uit?" << endl;
        cin >> input;

        switch (input) {
        case 'a':
            animate(grid, ANIMATE_TICK_COUNT, ANIMATE_TICK_PAUSE);
            break;
        case 't':
            animate(grid, 1, 0);
            break;
        case 'q':
            run = false;
            break;
        default:
            cout << "Invalid input." << endl;
            break;
        }
    };
}

int main() {

    //Get file name.
    string fileName;
    cout << "Grid input file: ";
    getline(cin, fileName);

    Grid<char> grid;

    //Try loading grid with data from given file.
    try{

        loadGrid(fileName, grid);
    }catch(const invalid_argument& e){

        cout << e.what();
        return -1;
    }

    //Clear console and render.
    clearConsole();
    renderGrid(grid);

    //Enter menu loop.
    menuLoop(grid);

    return 0;
}


