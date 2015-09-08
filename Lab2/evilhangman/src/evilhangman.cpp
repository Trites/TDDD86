#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
using namespace std;

const string alphabet  = "abcdefghijklmnopqrstuvwxyz";
const string DICT_PATH = "dictionary.txt";
const char EMPTY = '_';

/*
* Function: loadDictionary
* Usage: loadDictionary(fileName, dictionary(out))
* ---------------------------------------
* Loads the given unordered_set with words from a file.
* Note: This method makes assumptions about the format of the file, behaviour for non-standard format is undefined.
*/
void loadSortedDictionary(const string fileName, unordered_map<int, unordered_set<string>>& dictionary){


    ifstream dictStream{fileName};
    //for_each(istream_iterator<string>{dictStream}, istream_iterator<string>{}, insertByLegth(*__first, dictionary));

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

void printDict(const unordered_map<int, unordered_set<string>>& dictionary){

    for ( auto it = dictionary.begin(); it != dictionary.end(); ++it ){
        cout << it->first << endl;
        for (auto inner = it->second.begin(); inner != it->second.end(); ++inner){

            cout << *inner << endl;
        }
        cout << endl;
    }
}


void printDictSize(const unordered_set<string>& dictionary){

    cout << "There are " << dictionary.size() << " words left." << endl;
}

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

void partitionDictionary(const int wordLength, const char& partitionBy, const unordered_set<string>& dictionary, unordered_map<string, unordered_set<string>>& partitionedDict){

    partitionedDict.clear();
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

        if(partitionedDict.count(key) == 0){

            partitionedDict[key] = unordered_set<string>{};
        }


        partitionedDict[key].insert(word);
    }
}

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

    int wordLength;
    unordered_map<int, unordered_set<string>> sortedDictionary;
    unordered_set<string> dictionary;
    loadSortedDictionary(DICT_PATH, sortedDictionary);

    bool goodLength = false;

    while (!goodLength) {

        cout << "Enter word length: ";
        cin >> wordLength;
        goodLength = loadDictByLength(3, sortedDictionary, dictionary);
    }



    unordered_set<string> guessedLetters;
    printDictSize(dictionary);


    char guess;
    string progress;
    int missingLetters = wordLength;
    progress.assign(wordLength, '_');

    unordered_map<string, unordered_set<string>> partitionedDict;

    do{
        system("cls");
        cout << progress << endl;
        printDictSize(dictionary);
        cout << "Make a guess: ";
        cin >> guess;

        partitionDictionary(wordLength, guess, dictionary, partitionedDict);
        string familyKey = selectWordFamily(partitionedDict, dictionary);
        missingLetters -= updateProgress(familyKey, progress);



    }while(missingLetters > 0);


    cout << endl << progress  << endl << "You did it! The word was [" << progress << "] all along." << endl;
    //copy(begin(dictionary), end(dictionary), ostream_iterator<string>{cout, " "});


    // TODO: Finish the program!

    return 0;
}
