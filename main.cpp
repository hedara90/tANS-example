#include "tANS.h"

int main(int argc, char *argv[])
{
    std::string fileName;
    if (argc == 2)
        fileName = argv[1];
    else
        fileName = "testFile.bin";

    std::vector<int> input = readFileAsNibbles(fileName);
    EncodedData data;

    std::vector<int> symbols;
    std::vector<int> counts;

    std::vector<DecodeCol> decodeTable;
    std::vector<EncodeCol> encodeTable;

    symbols = findSymbols(input);
    counts = countSymbols(input, symbols);

    printf("Symbols and their counts\n");
    printSymbols(symbols);
    printCounts(counts);

    printf("Sorted symbols and their counts\n");
    sortCount(&counts, &symbols);
    printSymbols(symbols);
    printCounts(counts);

    counts = normalizeCounts(counts, 32);
    printf("Symbols and their normalized counts\n");
    printSymbols(symbols);
    printCounts(counts);

    decodeTable = createDecodingTable(symbols, counts);
    encodeTable = createEncodingTable(decodeTable, symbols);

    data = encodeData(input, encodeTable);

    std::vector<int> output = decodeData(&data, decodeTable, input.size());
    
    if (!areVectorsEqual(input, output))
        printf("Input and output doesn't match\n");
    else
        printf("Input and output matches\n");

    return 0;
}
