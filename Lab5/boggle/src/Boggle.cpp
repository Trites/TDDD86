// This is the .cpp file you will edit and turn in.
// We have provided a minimal skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header and replace it with your own

#include <sstream>
#include <stack>
#include <utility>
#include <string>
#include <vector>
#include <algorithm>
#include <ctype.h>
#include <stdio.h>
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

        board = { BOARD_HEIGHT, BOARD_WIDTH };
        lexicon = { DICTIONARY_FILE };

    }

    void Boggle::reset(){

        playerScore = 0;
        cpuScore = 0;
        playerWords.clear();
        cpuWords.clear();
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

    bool Boggle::buildTable(string& debugBoard){

        if(debugBoard.size() != BOARD_SIZE)
            return false;

        for(int i = 0; i < BOARD_SIZE; i++){

            if(!isalpha(debugBoard[i]))
                return false;

            debugBoard[i] = toupper(debugBoard[i]);

            board[i/BOARD_WIDTH][i % BOARD_WIDTH] = debugBoard[i];
        }
        return true;
    }

    /*
     * Resets valid words collection and explores the board.
     * Adds all valid words to the validWords collection.
     */
    void Boggle::findAllWords(){

        cpuWords.clear();
        unordered_set<int> visited;

        for(int y = 0; y < BOARD_HEIGHT; y++){
            for(int x = 0; x < BOARD_WIDTH; x++){

                explore("", x, y, visited);
            }
        }
    }

    /*
     * Print the board to console.
     */
    void Boggle::draw() const{

        for(int y = 0; y < BOARD_HEIGHT; y++){
            for(int x = 0; x < BOARD_WIDTH; x++){

                cout << board.get(y, x);
            }
            cout << "\n";
        }
        cout << endl;
    }

    /*
     * Returns the total score of the player.
     */
    int Boggle::getPlayerScore() const{

        return playerScore;
    }

    /*
     * Returns the computers score.
     */
    int Boggle::getCPUScore() const{

        return cpuScore;
    }

    set<string> Boggle::getCPUWords() const{

        return cpuWords;
    }

    /*
     * Returns all valid guesses the player has made on the currrent board so far.
     */
    set<string> Boggle::getPlayerWords() const{

        return playerWords;
    }

    /*
     * Checks a guess against the game board.
     * Returns true if guess is valid.
     * If guess is invalid the error parameter will contain an error.
     */
    bool Boggle::makeGuess(const string& guess, string& error){

        if(guess == "1"){

            for(auto& word : cpuWords)
                cout << word << endl;
            cout << endl;
            error = "DEBUG.";
            return false;
        }

        if(guess.length() < MIN_WORD_LENGTH){

            ostringstream oss;
            oss << "Word must be at least " << MIN_WORD_LENGTH << " letters long.";
            error = oss.str();
            return false;
        }

        if(playerWords.count(guess) != 0){

            error = "You've already guessed that word!";
            return false;
        }

        if(!lexicon.contains(guess)){

            error = "That word does not exist.";
            return false;
        }


        if(!findWord(guess)){

            error = "Word exists, but not on game board.";
            return false;
        }

        playerScore += scoreFor(guess);
        playerWords.insert(guess);
        return true;
    }

    /*
     * Explores all cells until given word is found.
     * Returns false if word is not found.
     */
    bool Boggle::findWord(const string& word) const
    {
        unordered_set<int> visited;
        for(int y = 0; y < BOARD_HEIGHT; y++){
            for(int x = 0; x < BOARD_WIDTH; x++){

                if(exploreFor(word, x, y, visited))
                    return true;
            }
        }

        return false;
    }

    /*
     * Explores the board from coordinates x,y.
     * Returns true if given word is found.
     */
    bool Boggle::exploreFor(const string& word, int x, int y, unordered_set<int>& visited, size_t charIndex) const
    {
        if(charIndex == word.length())
            return true;

        if(board.get(y, x) == word[charIndex]){


            //Convert coordinates to index in order to more easily store them as visited.
            int currentIndex = x + y * BOARD_WIDTH;
            visited.insert(currentIndex);

            //Check all possible neighbours, exploring those who are withing bounds.
            for(int ny = y - 1; ny <= y + 1; ny++){
                for(int nx = x - 1; nx <= x + 1; nx++){
                    if(board.inBounds(ny, nx)){
                        if(visited.count(nx + ny * BOARD_WIDTH) == 0){

                            if(exploreFor(word, nx, ny, visited, charIndex + 1))
                                return true;
                        }
                    }
                }
            }

            //Erase cell from visited collection as we are backtracking.
            visited.erase(currentIndex);
        }

        return false;
    }

    /*
     * Recursivly explores the game board and add any solutions to validWords collection.
     */
    void Boggle::explore(string word, int x, int y, unordered_set<int>& visited){

        //Add letter contained in this cell to word.
        word.push_back(board.get(y, x));

        //If our word is a valid prefix it is worth to continue, otherwise we return.
        if(lexicon.containsPrefix(word)){

            //If word is acceptable we add it to validWords collection.
            if(word.length() >= MIN_WORD_LENGTH && !alreadyFound(word) && lexicon.contains(word)){

                cpuScore += scoreFor(word);
                cpuWords.insert(word);
            }

            //Convert coordinates to index in order to more easily store them as visited.
            int currentIndex = x + y * BOARD_WIDTH;
            visited.insert(currentIndex);

            //Check all possible neighbours, exploring those who are withing bounds.
            for(int ny = y - 1; ny <= y + 1; ny++){
                for(int nx = x - 1; nx <= x + 1; nx++){
                    if(board.inBounds(ny, nx)){
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

    int Boggle::scoreFor(const string &word) const
    {
        return word.length() - (MIN_WORD_LENGTH - 1);
    }

    bool Boggle::alreadyFound(const string &word) const
    {
        return cpuWords.count(word) != 0 || playerWords.count(word) != 0;
    }
