#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

struct DecodeCol {
    int state;
    char symbol;
    int y;
    int k;
};

struct EncodeHelper {
    char symbol;
    std::vector<int> states;
    std::vector<int> yVals;
    std::vector<int> kVals;
    std::vector<int> yPrimVals;
    int currentRepeat = 0;
    int currentIndex = 0;
};

struct EncodeSymbolData {
    char symbol;
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

std::vector<DecodeCol> createDecodingTable(std::vector<char> symbols, std::vector<int> frequencies);

std::vector<EncodeCol> createEncodingTable(std::vector<DecodeCol> decodeTable, std::vector<char> symbols);

EncodedData encodeString(std::string input, std::vector<EncodeCol> encodingTable);
std::string decodeString(EncodedData *data, std::vector<DecodeCol> decodeTable, int numChars);

std::vector<char> findSymbols(std::string input);
std::vector<int> countSymbols(std::string input, std::vector<char> symbols);
std::vector<int> normalizeCounts(std::vector<int> counts, int tableSize);

void printEncodeTable(std::vector<EncodeCol> encodeTable, std::vector<char> symbols);
void printDecodeTable(std::vector<DecodeCol> decodeTable);
