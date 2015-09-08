#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <iterator>
#include <algorithm>
#include <unordered_set>
using namespace std;

const string alphabet  = "abcdefghijklmnopqrstuvwxyz";
const string DICT_PATH = "dictionary.txt";

/*
* Function: loadDictionary
* Usage: loadDictionary(fileName, dictionary(out))
* ---------------------------------------
* Loads the given unordered_set with words from a file.
* Note: This method makes assumptions about the format of the file, behaviour for non-standard format is undefined.
*/
void loadDictionary(const string fileName, unordered_set<string>& dictionary){

    ifstream dictStream{fileName};
    dictionary.insert(istream_iterator<string>{dictStream}, istream_iterator<string>{});
    dictStream.close();
}

/*
* Function: getNeighbours
* Usage: getNeighbours(word, dictionary, neighbours(out))
* ---------------------------------------
* Loads the given vector with all neighbours of the given word.
* A word does not count as its own neighbour.
*/
void getNeighbours(const string& word, const unordered_set<string>& dictionary, vector<string>& neighbours){

    int wordLength = word.length();
    string newWord;

    for(int i = 0; i < wordLength; i++){
        newWord = word;
        for(char letter : alphabet){

            //Check to make sure we do not add a word as its own neighbour.
            if(letter != word[i]){
                newWord[i] = letter;
                //Checks to make sure that the generated word exists in dictionary.
                if(dictionary.count(newWord) != 0){
                    neighbours.push_back(newWord);
                }
            }
        }
    }
}

/*
* Function: consumePrintStack
* Usage: consumePrintStack(stackToPrint(out))
* ---------------------------------------
* Prints the content of the given stack in order top to bottom.
* Note: The given stack will be emptied to avoid doing an unnecessary copy.
*/
void consumePrintStack(stack<string>& stackToPrint){

    for (string s; !stackToPrint.empty(); stackToPrint.pop())
            cout << s << endl;
}

/*
* Function: findChain
* Usage: findChain(origin, target, dictionary)
* ---------------------------------------
* Prints the shortest word chain for the given words, if one exists.
*/
void findChain(const string& origin, const string& target, const unordered_set<string>& dictionary){

    unordered_set<string> visited_words;
    visited_words.insert(origin);

    queue<stack<string>> chainQueue;
    chainQueue.push(stack<string>());
    chainQueue.front().push(origin);

    while(!chainQueue.empty()){

        stack<string> currentStack = chainQueue.front();
        chainQueue.pop();
        if(currentStack.top() == target){

            cout << "Word chain found:" << endl;
            consumePrintStack(currentStack);
            return;
        }
        else{
            vector<string> neighbours;
            getNeighbours(currentStack.top(), dictionary, neighbours);

            for(string w : neighbours){

                if(visited_words.count(w) == 0){

                    stack<string> currentStackCopy = currentStack;
                    currentStackCopy.push(w);
                    chainQueue.push(currentStackCopy);
                    visited_words.insert(w);
                }
            }
        }
    }

    cout << "Failed to find a word chain." << endl;
}

int main() {
    cout << "Welcome to TDDD86 Word Chain." << endl;
    cout << "If you give me two English words, I will transform the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;
    cout << "Please type two words: ";

    //Using unordered_set as dictionary since we are only interested in checking if a word exists, unordered_set does this in O(1).
    unordered_set<string> dictionary;
    loadDictionary(DICT_PATH, dictionary);

    //Get origin and target words.
    string origin;
    string target;
    cin >> origin;
    cin >> target;

    //Check for obvious non-solvable cases.
    if(origin.length() != target.length()){

        cout << "Words must be of equal length!" << endl;
        return -1;
    }
    if(dictionary.count(origin) == 0){

        cout << "Origin word " << origin << " does not exist in dictionary." << endl;
        return -1;
    }
    if(dictionary.count(target) == 0){

        cout << "Target word " << target << " does not exist in dictionary." << endl;
        return -1;
    }

    //Try to find chain from origin to target.
    findChain(origin, target, dictionary);
    return 0;
}
