#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include <unordered_set>
#include <unordered_map>
using namespace std;

const string alphabet  = "abcdefghijklmnopqrstuvwxyz";
const string DICT_PATH = "dictionary.txt";
const char EMPTY = '_';

/*
 * Loads the given map with words from a file, the length of the word becomes the key in the map.
*/
void loadSortedDictionary(const string fileName, unordered_map<int, unordered_set<string>>& dictionary){

    ifstream dictStream{fileName};

    string word;
    while(dictStream){

        getline(dictStream, word);
        int wordLength = word.length();

        if(dictionary.count(wordLength) == 0){

            dictionary[wordLength] = unordered_set<string>{};
        }

        dictionary[wordLength].insert(word);
    }

    dictStream.close();
}

/*
 * Loads the given set with all words of the given length if it exists in the map.
 * Returns true if words of the given length exists, returns false if none can be found.
*/
bool loadDictByLength(const int wordLength, unordered_map<int, unordered_set<string>> sortedDictionary, unordered_set<string>& dictionary){

    if(wordLength > 0){
        if(sortedDictionary.count(wordLength) > 0){

            dictionary = sortedDictionary[wordLength];
            return true;
        }
    }

    cout << "I do not know any words of length: " << wordLength << endl;
    return false;
}

void printDictSize(const unordered_set<string>& dictionary){

    cout << "There are " << dictionary.size() << " words left." << endl;
}


/*
 * Divides the given set of words into families based on a given letter. It then loads a map with these families where the signature of the family is the key.
*/
void partitionDictionary(const int wordLength, const char& partitionBy, const unordered_set<string>& dictionary, unordered_map<string, unordered_set<string>>& destinationDict){

    destinationDict.clear();
    string key;
    key.resize(wordLength);
    for(string word : dictionary){
        for(int i = 0; i < wordLength; i++){

            if(word[i] == partitionBy){
                key[i] = partitionBy;

            }else{
                key[i] = EMPTY;
            }
        }

        if(destinationDict.count(key) == 0){

            destinationDict[key] = unordered_set<string>{};
        }

        destinationDict[key].insert(word);
    }
}

/*
 * Selects the word family containing the highest word count.
*/
string selectWordFamily(unordered_map<string, unordered_set<string>>& partitionedDict, unordered_set<string>& dictionary){

    int largest = 0;
    string largestKey;

    for ( auto it = partitionedDict.begin(); it != partitionedDict.end(); ++it ){
        if(it->second.size() > largest){
            largest = it->second.size();
            largestKey = it->first;
        }
    }

    dictionary = partitionedDict[largestKey];
    return largestKey;
}

/*
 * Updates the progress string with any new letters found.
 * Returns number of new letters.
*/
int updateProgress(const string& newKey, string& progress){

    int lettersFound = 0;
    int length = progress.size();

    for(int i = 0; i < length; i++){

        if(newKey[i] != EMPTY){

            progress[i] = newKey[i];
            lettersFound++;
        }
    }

    return lettersFound;
}

int main() {
    cout << "Welcome to Hangman." << endl;

    //Init dictionary

    unordered_map<int, unordered_set<string>> sortedDictionary;
    unordered_set<string> dictionary;
    loadSortedDictionary(DICT_PATH, sortedDictionary);

    //Get desired length of word
    bool goodLength = false;
    int wordLength;
    while (!goodLength) {

        cout << "Enter word length: ";
        cin >> wordLength;
        goodLength = loadDictByLength(wordLength, sortedDictionary, dictionary);
    }

    int guessCount;
    cout << "Enter the number of guesses you need: ";
    cin >> guessCount;

    unordered_set<char> guessedLetters; //For storing already guessed letters.
    unordered_map<string, unordered_set<string>> partitionedDict; //Will be used to store word sets partitioned by family signature.

    char guess;
    string progress;
    progress.assign(wordLength, '_');
    int missingLetters = wordLength;


    do{
        system("cls");

        //User can enter '1' to debug the current state of the program, this will print all words in the current word set and the size of the set.
        if(guess == '1'){
            copy(begin(dictionary), end(dictionary), ostream_iterator<string>{cout, " "});
            cout << "\n\n";
            printDictSize(dictionary);
        }

        //Print previously guessed letters and ask the player to enter a new guess.
        cout << progress << endl;
        copy(begin(guessedLetters), end(guessedLetters), ostream_iterator<char>{cout, " "});
        cout << "You have " << guessCount << " guesses left." << endl << "Make a guess: ";
        cin >> guess;

        //If the guess is a letter and not already in the guessed letters set we consider it valid and updates the word set based on the guess.
        if(isalpha(guess) && guessedLetters.count(guess) == 0){

            guessedLetters.insert(guess);
            partitionDictionary(wordLength, guess, dictionary, partitionedDict);
            string familyKey = selectWordFamily(partitionedDict, dictionary);

            int lettersFoundCount = updateProgress(familyKey, progress);

            if(lettersFoundCount > 0){

                missingLetters -= lettersFoundCount;
            }else{

                guessCount--;
            }
        }

    }while(missingLetters > 0 && guessCount > 0); //While there are still unkown letters and guesses left, we continue.

    //If there are no missing letters we congratulate the player, otherwise we scold the player.
    if(missingLetters > 0){

        cout << "Wrong, sir, wrong! The word was clearly '" << *begin(dictionary) << "'. You get nothing, you loose!" << endl;
    }else{

        cout << endl << progress  << endl << "You did it! The word was " << progress << " all along." << endl;
    }
    return 0;
}
