#include "TileList.h"
#include <algorithm>

static const int MAX_SIZE = 10;

TileList::TileList()
{
    tiles = new Tile[MAX_SIZE];
    count = 0;
}

TileList::~TileList()
{
    delete [] tiles;
}

/*
* Adds a tile at the end of the array if space is available.
*/
void TileList::addTile(Tile tile)
{
    if(count < MAX_SIZE){
        tiles[count] = tile;
        count++;
    }
}


/*
* Draws all tiles in order of first to last.
*/
void TileList::drawAll(QGraphicsScene* scene)
{
    for(int i = 0; i < count; i++){

        tiles[i].draw(scene);
    }
}

/*
* Returns the index of the top tile that contains the given coordinates.
*/
int TileList::indexOfTopTile(int x, int y)
{
    for(int i = count - 1; i >= 0; i--){

        if(tiles[i].contains(x, y)){

            return i;
        }
    }

    return -1;
}

/*
* Moves a tile to the top by moving it to the end of the array.
*/
void TileList::raise(int x, int y)
{
    int targetIndex = indexOfTopTile(x, y);

    if(targetIndex != -1){

        Tile temp = tiles[targetIndex];
        shiftRange(targetIndex, count-1);
        tiles[count-1] = temp;
    }
}

/*
* Moves a tile to the bottom by moving it to the begining of the array.
*/
void TileList::lower(int x, int y)
{
    int targetIndex = indexOfTopTile(x, y);

    if(targetIndex != -1){

        Tile temp = tiles[targetIndex];
        shiftRange(targetIndex, 0);
        tiles[0] = temp;
    }
}

/*
* Removes the top tile that contains the given coordinates.
*/
void TileList::remove(int x, int y)
{
    int targetIndex = indexOfTopTile(x, y);

    if(targetIndex != -1){

        shiftRange(targetIndex, count-1);
        count--;
    }
}

/*
* Removes all tiles containing the given coordinates.
*/
void TileList::removeAll(int x, int y)
{
    Tile newArr [MAX_SIZE];
    int newCount = 0;

    for(int i = 0; i < count; i++){

        if(!tiles[i].contains(x, y)){

            newArr[newCount] = tiles[i];
            newCount++;
        }
    }

    copy(begin(newArr), end(newArr), tiles);
    count = newCount;
}

/*
* Will shift the given range one step to the left or right depending on given indices.
* Note: Element with index of <code>indexBegins</code> will be overwritten, <code>indexEnd</code> will be duplicated.
*/
void TileList::shiftRange(int indexBegin, int indexEnd)
{
    int delta = indexEnd - indexBegin;
    int direction = (0 < delta) - (delta < 0);

    for(int i = indexBegin; i != indexEnd; i += direction){

        tiles[i] = tiles[i + direction];
    }
}
