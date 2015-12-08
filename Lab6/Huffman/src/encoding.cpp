// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include <algorithm>
#include <iterator>
#include <queue>

map<int, int> buildFrequencyTable(istream& input) {

    map<int, int> freqTable;

    std::istream_iterator<int> it {input};
    std::istream_iterator<int> eos;

    //Read input and populate frequency table
    while(it != eos){

        freqTable[*it] += 1;
        ++it;
    }

    return freqTable;
}

HuffmanNode* buildEncodingTree(const map<int, int> &freqTable) {

    priority_queue<HuffmanNode*> prioQueue;

    for(map<int, int>::const_iterator it = begin(freqTable); it != end(freqTable); ++it){

        prioQueue.push(new HuffmanNode(it->first, it->second));
    }

    HuffmanNode *root = new HuffmanNode();

    while(!prioQueue.empty()){

        HuffmanNode *node = prioQueue.top();
        prioQueue.pop();

        if(root->)
    }
    return nullptr;
}

map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    // TODO: implement this function
    map<int, string> encodingMap;
    return encodingMap;
}

void encodeData(istream& input, const map<int, string> &encodingMap, obitstream& output) {
    // TODO: implement this function
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    // TODO: implement this function
}

void compress(istream& input, obitstream& output) {

    map<int, int> frequencyTable = buildFrequencyTable(input);

    HuffmanNode *encodingTree = buildEncodingTree(frequencyTable);


}

void decompress(ibitstream& input, ostream& output) {
    // TODO: implement this function
}

void freeTree(HuffmanNode* node) {
    // TODO: implement this function
}
