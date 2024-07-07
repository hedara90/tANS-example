#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

struct DecodeCol {
    int state;
    int symbol;
    int y;
    int k;
};

struct EncodeHelper {
    int symbol;
    std::vector<int> states;
    std::vector<int> yVals;
    std::vector<int> kVals;
    std::vector<int> yPrimVals;
    int currentRepeat = 0;
    int currentIndex = 0;
};

struct EncodeSymbolData {
    int symbol;
    int nextState;
    int streamValue;
    int numBits;
};
struct EncodeCol {
    int state;
    std::vector<EncodeSymbolData> symbols;
};

struct EncodedData {
    int initialState;
    std::vector<bool> bitStream;
};

std::vector<DecodeCol> createDecodingTable(std::vector<int> symbols, std::vector<int> frequencies);

std::vector<EncodeCol> createEncodingTable(std::vector<DecodeCol> decodeTable, std::vector<int> symbols);

EncodedData encodeData(std::vector<int> input, std::vector<EncodeCol> encodingTable);
std::vector<int> decodeData(EncodedData *data, std::vector<DecodeCol> decodeTable, int numChars);

std::vector<int> findSymbols(std::vector<int> input);
std::vector<int> countSymbols(std::vector<int> input, std::vector<int> symbols);
std::vector<int> normalizeCounts(std::vector<int> counts, int tableSize);
void sortCount(std::vector<int> *counts, std::vector<int> *symbols);

void printEncodeTable(std::vector<EncodeCol> encodeTable, std::vector<int> symbols);
void printDecodeTable(std::vector<DecodeCol> decodeTable);
void printCounts(std::vector<int> count);
void printSymbols(std::vector<int> symbols);

std::vector<int> readFileAsNibbles(std::string filePath);
bool areVectorsEqual(std::vector<int> input, std::vector<int> output);
