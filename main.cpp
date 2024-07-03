#include "tANS.h"

int main()
{
    //  String of symbols to encode
    std::string testString = "ABACCACBC";
    //  List of unique symbols to encode
    std::vector<char> symbols = {'A', 'B', 'C'};
    //  List of frequencies of the symbols in the same order, normalized so the total is a power of 2
    std::vector<int> frequencies = {3, 3, 2};

    std::cout << "Original string: " << testString << "\n";

    std::vector<DecodeCol> decodeTable = createDecodingTable(symbols, frequencies);

    //  Uncomment to view decoding table
    /*
    for (DecodeCol currCol : decodeTable)
    {
        std::cout << "State: " << currCol.state << " ->Symbol: " << currCol.symbol << " ->y, k " << currCol.y << "," << currCol.k << "\n";
    }
    */
    std::vector<EncodeCol> encodeTable = createEncodingTable(decodeTable, symbols);

    //  Uncomment to view encoding table
    /*
    printf("x: ");
    for (int i = 0; i < encodeTable.size(); i++)
    {
        printf("%2zu ", i+encodeTable.size());
    }
    printf("\n");
    for (int i = 0; i < symbols.size(); i++)
    {
        printf("s: ");
        for (int k = 0; k < encodeTable.size(); k++)
        {
            printf("%2i ", encodeTable[k].symbols[i].nextState);
        }
        printf("\n");
        printf("b: ");
        for (int k = 0; k < encodeTable.size(); k++)
        {
            printf("%2i ", encodeTable[k].symbols[i].streamValue);
        }
        printf("\n");
        printf("k: ");
        for (int k = 0; k < encodeTable.size(); k++)
        {
            printf("%2i ", encodeTable[k].symbols[i].numBits);
        }
        printf("\n");
    }
    */

    EncodedData data = encodeString(testString, encodeTable);
    std::string decodedString = decodeString(&data, decodeTable, testString.size());
    std::cout << "Decoded string:  " << decodedString << "\n";
    return 0;
}
