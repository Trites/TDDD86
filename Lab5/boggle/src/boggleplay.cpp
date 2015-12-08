// You will edit and turn in this CPP file.
// Also remove these comments here and add your own.
// TODO: remove this comment header and replace with your own

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Boggle.h"
#include "bogglemain.h"
#include "strlib.h"
// TODO: include any other header files you need


/*
 * Prints current player score, guessed words and board.
 */
void printPlayerView(Boggle& boggle){

    cout << "Your score: " << boggle.getPlayerScore() << endl;
    cout << endl << "Your words (" << boggle.getPlayerWords().size() << "): " << endl;

    for(auto& word : boggle.getPlayerWords())
        cout << word << endl;

    boggle.draw();
}


/*
 * Takes a guess from the player and looks for it on the board.
 * Returns false if player enters empty string.
 */
bool playerTurn(Boggle& boggle){

    string input;
    string error;

    cout << endl << "Make a guess: ";

    getline(cin, input);;

    if(input == "")
        return false;

    if(boggle.makeGuess(input, error)){

        cout << "You found new word: " << input << endl;

    }else{

        cout << error << endl;
    }

    return true;
}

/*
 * Finds and prints all words not yet found by player.
 */
void cpuTurn(Boggle& boggle){

    boggle.findAllWords();
    set<string> result = boggle.getCPUWords();

    cout << "My words(" << result.size() << "):" << endl;


    for(auto& word : result)
        cout << word << endl;
}


/*
 * Plays one game of Boggle using the given boggle game state object.
 */
void playOneGame(Boggle& boggle) {

    boggle.reset();

    bool validBoard = false;
    while(!validBoard){

        cout << "Build custom board? Leave empty for random board." << endl;
        cout << "Board: ";

        string customBoard;
        getline(cin, customBoard);

        if(customBoard == ""){

            cout << "Random board!" << endl;
            boggle.buildTable();
            validBoard = true;
        }
        else{

            validBoard = boggle.buildTable(customBoard);
        }
    }


    cout << "Your turn!" << "\n\n";



    do{

        printPlayerView(boggle);
    }while (playerTurn(boggle));


    cpuTurn(boggle);


    cout << "My score: " << boggle.getCPUScore() << endl;
    cout << "Your score: " << boggle.getPlayerScore() << endl;

    if(boggle.getCPUScore() > boggle.getPlayerScore()){

        cout << "You have been destroyed!" << endl;
    }else{

        cout << "You win." << endl;
    }
}

/*
 * Erases all currently visible text from the output console.
 */
void clearConsole() {
#if defined(_WIN32) || defined(_WIN64)
    std::system("CLS");
#else
    // assume POSIX
    std::system("clear");
#endif
}
