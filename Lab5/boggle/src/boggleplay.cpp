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
 * Plays one game of Boggle using the given boggle game state object.
 */
void playOneGame(Boggle& boggle) {
    // TODO: implement this function (and add any other functions you like to help you)

    boggle.buildTable();
    boggle.findAllWords();

    cout << "Your turn!" << "\n\n";

    string input;
    string error;
    do{

        boggle.draw();

        cout << "Make a guess: ";

        getline(cin, input);

        cout << "\n\n";



        if(input == "")
            break;


        if(boggle.makeGuess(input, error)){

            cout << "You found new word: " << input << endl;

        }else{

            cout << error << endl;
        }

        cout << "Your score: " << boggle.getPlayerScore() << endl;

    }while (true);

    cout << "My words: " << endl;

    unordered_set<string> result = boggle.getCPUWords();

    for(auto& word : result)
        cout << word << endl;

    cout << "My score: " << boggle.getCPUScore() << endl;

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