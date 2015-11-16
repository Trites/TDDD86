// This is the .h file you will edit and turn in.
// We have provided a minimal skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own, as well as on the members.
// TODO: remove this comment header and replace it with your own

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include "lexicon.h"
#include "grid.h"
// TODO: include any other header files you need

using namespace std;

class Boggle {
public:
    const string DICTIONARY_FILE = "EnglishWords.dat";
    const unsigned int MIN_WORD_LENGTH = 4;
    const int BOARD_WIDTH = 4;
    const int BOARD_HEIGHT = 4;
    const int BOARD_SIZE = BOARD_WIDTH * BOARD_HEIGHT;


    Boggle();
    ~Boggle() = default;
    void buildTable();
    void findAllWords();
    void draw() const;
    bool makeGuess(const string& guess, string& error);
    int getPlayerScore() const;
    int getCPUScore() const;
    unordered_set<string> getPlayerWords() const;
    unordered_set<string> getCPUWords() const;

private:
    Grid<char> board;
    Lexicon lexicon;
    unordered_set<string> validWords;
    unordered_set<string> validGuesses;

    void explore(string word, int x, int y, unordered_set<int>& visited);

};

#endif
