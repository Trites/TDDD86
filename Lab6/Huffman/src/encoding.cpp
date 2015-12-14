// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include <algorithm>
#include <iterator>
#include <queue>
#include <stack>

map<int, int> buildFrequencyTable(istream& input) {

    map<int, int> freqTable;

    input.unsetf(ios_base::skipws);
    std::istream_iterator<char> it {input};
    std::istream_iterator<char> eos;

    //Read input and populate frequency table
    while(it != eos){

        cout << "Found: " << *it << endl;
        freqTable[*it] += 1;
        ++it;
    }

    freqTable[PSEUDO_EOF] = 1;

    return freqTable;
}

template<typename Type, typename Compare = std::less<Type> >
struct pless : public std::binary_function<Type *, Type *, bool> {
    bool operator()(const Type *x, const Type *y) const
        { return Compare()(*x, *y); }
};

HuffmanNode* buildEncodingTree(const map<int, int> &freqTable) {

    priority_queue<HuffmanNode*, vector<HuffmanNode*>, pless<HuffmanNode>> prioQueue;

    for(map<int, int>::const_iterator it = begin(freqTable); it != end(freqTable); ++it){

        cout << "Build node: " << it->first << " " << it->second << endl;
        prioQueue.push(new HuffmanNode(it->first, it->second));
    }

    //While there are at least one pair left
    while(prioQueue.size() > 1){

        HuffmanNode *zero = prioQueue.top();
        prioQueue.pop();
        HuffmanNode *one = prioQueue.top();
        prioQueue.pop();

        cout << "Merging: " << zero->character << " " << one->character << endl;
        HuffmanNode* root = new HuffmanNode(NOT_A_CHAR, zero->count + one->count, zero, one);
        prioQueue.push(root);
    }

    return prioQueue.top();
}

void buildEncodingMap(HuffmanNode* encodingTree, string encoding, map<int, string>& result) {

    if(encodingTree->isLeaf()){
        result[encodingTree->character] = encoding;
        return;
    }

    buildEncodingMap(encodingTree->zero, encoding + '0', result);
    buildEncodingMap(encodingTree->one, encoding + '1', result);
}

map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {

    map<int, string> encodingMap;

    buildEncodingMap(encodingTree, "", encodingMap);

    return encodingMap;
}

void encodeData(istream& input, const map<int, string> &encodingMap, obitstream& output) {

    input.unsetf(ios_base::skipws);
    vector<int> inputVector {istream_iterator<char>(input), istream_iterator<char>()};
    inputVector.push_back(PSEUDO_EOF);
    encodeData(inputVector, encodingMap, output);
}

void encodeData(vector<int>& input, const map<int, string> &encodingMap, obitstream& output) {

    cout << "Encoding " << input.size() << " tokens..." << endl;
    for(const int& token : input){

        char c = token;
        cout << "Encoding: " << c << " : " << encodingMap.at(token) << endl;
        for(const char& bit : encodingMap.at(token)){

            cout << bit-48;
            output.writeBit(bit - 48);
        }
        cout << endl;
    }
}

void decodeChar(ibitstream &input, HuffmanNode *encodingTree, ostream &output)
{

    if(encodingTree->isLeaf()){

        char c = encodingTree->character;
        output << c;
        //printf(output, "%c", encodingTree->character);
        cout << endl << "Decoded: " << c << endl;
        return;
    }

    switch(input.readBit()){

        case 0:
            cout << 0;
            decodeChar(input, encodingTree->zero, output);
            break;
        case 1:
            cout << 1;
            decodeChar(input, encodingTree->one, output);
            break;
    }
}

int decodeChar(ibitstream& input, HuffmanNode* encodingTree){

    if(encodingTree->isLeaf()){

        char c = encodingTree->character;
        cout << "Decoded: " << c << endl;
        return encodingTree->character;
    }

    if(input.readBit() == 0){

        return decodeChar(input, encodingTree->zero);
    }else{

        return decodeChar(input, encodingTree->one);
    }
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {

    int i;

    while(true){

        i = decodeChar(input, encodingTree);

        if(i == PSEUDO_EOF)
            return;

        output << static_cast<char>(i);
    }

    /*
    while(input.peek() != -1){

        decodeChar(input, encodingTree, output);
    }*/
}

void compress(istream& input, obitstream& output) {

    //vector<int> inputVector {istream_iterator<char>(input), istream_iterator<char>()};
    map<int, int> frequencyTable = buildFrequencyTable(input);
    saveFreqTable(frequencyTable);

    cout << frequencyTable.size() << endl;
    HuffmanNode *encodingTree = buildEncodingTree(frequencyTable);
    map<int, string> encodingMap = buildEncodingMap(encodingTree);

    input.clear();
    input.seekg(0);
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

void decompress(ibitstream& input, ostream& output) {

    map<int, int> freqTable = loadFreqTable();


    for (map<int,int>::const_iterator it=begin(freqTable); it!=end(freqTable); ++it)
        cout << "F-Table-A: " << it->first << " " << it->second << endl;

    HuffmanNode *encodingTree = buildEncodingTree(freqTable);
    map<int, string> encodingMap = buildEncodingMap(encodingTree);


    decodeData(input, encodingTree, output);
    freeTree(encodingTree);
}

void freeTree(HuffmanNode* node) {

    if(!node->isLeaf()){

        freeTree(node->zero);
        freeTree(node->one);
    }

    delete node;
}

void saveFreqTable(const map<int, int> &freqTable)
{
    ofstream file;
    file.open("freqTable.txt", ios::trunc);

    for (auto it=begin(freqTable); it!=end(freqTable); ++it)
        file << it->first << ' ' << it->second << '\n';

    file.close();

}

map<int, int> loadFreqTable()
{

    map<int, int> freqTable;

    ifstream file("freqTable.txt");
    string line;

    while(getline(file, line)){

        istringstream iss(line);

        int key;
        int value;

        iss >> key;
        iss >> value;

        cout << "F-table: " << key << " " << value << endl;
        freqTable[key] = value;

    }

    return freqTable;
}
