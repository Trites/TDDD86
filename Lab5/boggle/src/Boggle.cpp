// This is the .cpp file you will edit and turn in.
// We have provided a minimal skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header and replace it with your own

#include <sstream>
#include <unordered_set>
#include <stack>
#include <utility>
#include <string>
#include <vector>
#include <algorithm>
#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include "strlib.h"

static const int NUM_CUBES = 16;   // the number of cubes in the game
static const int CUBE_SIDES = 6;   // the number of sides on each cube
static string CUBES[NUM_CUBES] = {        // the letters on all 6 sides of every cube
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

    Boggle::Boggle(){

        board = { BOARD_WIDTH, BOARD_HEIGHT };
        lexicon = { DICTIONARY_FILE };
    }

    /*
     * Populates the board with random letters and shuffles them.
     */
    void Boggle::buildTable(){

        int i = 0;
        for(auto& cell : board){

            cell = CUBES[i][rand() % (CUBE_SIDES - 1)];
            i++;
        }

        shuffle(board);
    }

    /*
     * Resets valid words collection and explores the board.
     * Adds all valid words to the validWords collection.
     */
    void Boggle::findAllWords(){

        validWords.clear();
        unordered_set<int> visited;
        explore("", 1, 1, visited);
    }

    /*
     * Print the board to console.
     */
    void Boggle::draw() const{

        for(int y = 0; y < BOARD_HEIGHT; y++){
            for(int x = 0; x < BOARD_WIDTH; x++){

                cout << board.get(x, y);
            }
            cout << "\n";
        }
        cout << endl;
    }

    /*
     * Returns the total score of the player.
     */
    int Boggle::getPlayerScore() const{

        return validGuesses.size();
    }

    /*
     * Returns the computers score.
     */
    int Boggle::getCPUScore() const{

        return validWords.size() - validGuesses.size();
    }

    unordered_set<string> Boggle::getCPUWords() const{

        unordered_set<string> result {validWords};

        if(validGuesses.size() > 0)
            result.erase(begin(validGuesses));

        return result;
    }

    /*
     * Returns all valid guesses the player has made on the currrent board so far.
     */
    unordered_set<string> Boggle::getPlayerWords() const{

        return validGuesses;
    }

    /*
     * Checks a guess against the game board.
     * Returns true if guess is valid.
     * If guess is invalid the error parameter will contain an error.
     */
    bool Boggle::makeGuess(const string& guess, string& error){

        if(guess.length() < MIN_WORD_LENGTH){

            ostringstream oss;
            oss << "Word must be at least " << MIN_WORD_LENGTH << " letters long.";
            error = oss.str();
            return false;
        }

        if(validWords.count(guess) == 0){

            error = "That word does not exist.";
            return false;
        }

        if(validGuesses.count(guess) != 0){

            error = "You've already guessed that word!";
            return false;
        }

        validGuesses.insert(guess);
        return true;
    }

    /*
     * Recursivly explores the game board and add any solutions to validWords collection.
     */
    void Boggle::explore(string word, int x, int y, unordered_set<int>& visited){

        //Add letter contained in this cell to word.
        word.push_back(board.get(x, y));

        //If our word is a valid prefix it is worth to continue, otherwise we return.
        if(lexicon.containsPrefix(word)){

            //If word is acceptable we add it to validWords collection.
            if(word.length() >= MIN_WORD_LENGTH && lexicon.contains(word))
                validWords.insert(word);

            //Convert coordinates to index in order to more easily store them as visited.
            int currentIndex = x + y * BOARD_WIDTH;
            visited.insert(currentIndex);

            //Check all possible neighbours, exploring those who are withing bounds.
            for(int ny = y - 1; ny <= y + 1; ny++){
                for(int nx = x - 1; nx <= x + 1; nx++){
                    if(board.inBounds(nx, ny)){
                        if(visited.count(nx + ny * BOARD_WIDTH) == 0){

                                explore(word, nx, ny, visited);
                        }
                    }
                }
            }

            //Erase cell from visited collection as we are backtracking.
            visited.erase(currentIndex);
        }

        return;
    }
