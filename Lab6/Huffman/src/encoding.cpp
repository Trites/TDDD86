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

        freqTable[*it] += 1;
        ++it;
    }

    //Put EOF
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

    //Populate priority queue
    for(map<int, int>::const_iterator it = begin(freqTable); it != end(freqTable); ++it){

        prioQueue.push(new HuffmanNode(it->first, it->second));
    }

    //While there are at least one pair left
    while(prioQueue.size() > 1){

        //Take first pair in queue
        HuffmanNode *zero = prioQueue.top();
        prioQueue.pop();
        HuffmanNode *one = prioQueue.top();
        prioQueue.pop();

        //Merge pair and put their root back in the queue
        HuffmanNode* root = new HuffmanNode(NOT_A_CHAR, zero->count + one->count, zero, one);
        prioQueue.push(root);
    }

    //Top is the root of the entire tree
    return prioQueue.top();
}

void buildEncodingMap(HuffmanNode* encodingTree, string encoding, map<int, string>& result) {

    //Once we found a leaf we add the path as encoding for the character
    if(encodingTree->isLeaf()){
        result[encodingTree->character] = encoding;
        return;
    }

    //Traverse tree, adding 0 or 1 to encoding path as we go
    buildEncodingMap(encodingTree->zero, encoding + '0', result);
    buildEncodingMap(encodingTree->one, encoding + '1', result);
}

map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {

    map<int, string> encodingMap;
    buildEncodingMap(encodingTree, "", encodingMap);

    return encodingMap;
}

//TODO: Read one token at a time
void encodeData(istream& input, const map<int, string> &encodingMap, obitstream& output) {

    //Get one char and encode it
    char c;
    while(input.get(c)){

        encodeChar(c, encodingMap, output);
    }

    //Put EOF
    encodeChar(PSEUDO_EOF, encodingMap, output);
}

void encodeChar(int c, const map<int, string> &encodingMap, obitstream& output) {


    //Write the encoded char bit-by-bit to output
    for(const char& bit : encodingMap.at(c)){

        output.writeBit(bit - 48);
    }
}

int decodeChar(ibitstream& input, HuffmanNode* encodingTree){

    //If node is leaf then we have decoded a char
    if(encodingTree->isLeaf()){

        return encodingTree->character;
    }

    //Traverse tree depending on the next bit
    if(input.readBit() == 0){

        return decodeChar(input, encodingTree->zero);
    }else{

        return decodeChar(input, encodingTree->one);
    }
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {

    int c;
    while(true){

        c = decodeChar(input, encodingTree);

        //Return when EOF is found
        if(c == PSEUDO_EOF)
            return;

        //Write decoded char to outstream
        output << static_cast<char>(c);
    }
}

void compress(istream& input, obitstream& output) {

    //Build and save frequencyTable
    map<int, int> frequencyTable = buildFrequencyTable(input);
    saveFreqTable(frequencyTable);

    //Build tree and map
    HuffmanNode *encodingTree = buildEncodingTree(frequencyTable);
    map<int, string> encodingMap = buildEncodingMap(encodingTree);

    //Reset stream to beginning for second pass
    input.clear();
    input.seekg(0);

    //Encode data in stream and free tree
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

void decompress(ibitstream& input, ostream& output) {

    //Load frequencyTable from file
    map<int, int> freqTable = loadFreqTable();

    //Build tree and map
    HuffmanNode *encodingTree = buildEncodingTree(freqTable);
    map<int, string> encodingMap = buildEncodingMap(encodingTree);

    //Decode data and free tree
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

        freqTable[key] = value;

    }

    return freqTable;
}
