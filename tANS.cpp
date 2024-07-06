#include "tANS.h"

std::vector<DecodeCol> createDecodingTable(std::vector<char> symbols, std::vector<int> frequencies)
{
    std::vector<DecodeCol> decodeTable;

    int totalFreq = 0;
    std::vector<int> yValueCounting = frequencies;

    for (int freq : frequencies)
        totalFreq += freq;

    int indexCounter = 0;
    //  Create a table with size equal to the total frequencies
    for (int i = 0; i < totalFreq; i++)
    {
        DecodeCol currCol;
        int currentSymbolIndex = indexCounter % symbols.size();

        //  Set the state to total frequencies + current index
        currCol.state = totalFreq + i;
        //  Set the symbol to the next valid symbol
        currCol.symbol = symbols[currentSymbolIndex];
        //  Set the y-value to the frequency of the symbol + number of times used
        currCol.y = yValueCounting[currentSymbolIndex];
        //  Calculate the k-value, where k is how many times y needs to be doubled
        //  to be larger or equal to the total frequency count
        int k = 0;
        int yDoubledk = currCol.y << k;
        while (yDoubledk < totalFreq)
        {
            k++;
            yDoubledk = currCol.y << k;
        }
        currCol.k = k;
        decodeTable.push_back(currCol);

        //  Update the y-value counter
        yValueCounting[currentSymbolIndex]++;
        //  Decrease the freq for the symbol of the current table entry
        frequencies[currentSymbolIndex]--;

        //  Find the next valid symbol to generate a table entry for
        indexCounter++;
        for (int j = 0; j < symbols.size(); j++)
        {
            if (frequencies[indexCounter % symbols.size()] != 0)
                break;
            else
                indexCounter++;
        }
    }
    return decodeTable;
}

std::vector<EncodeCol> createEncodingTable(std::vector<DecodeCol> decodeTable, std::vector<char> symbols)
{
    std::vector<EncodeCol> encodeTable;
    encodeTable.resize(decodeTable.size());
    for (int i = 0; i < decodeTable.size(); i++)
    {
        encodeTable[i].state = decodeTable.size() + i;
    }
    std::vector<EncodeHelper> encodeHelpers;

    //  Build the encoding helper tables, one per symbol
    for (char symbol : symbols)
    {
        EncodeHelper newHelper;
        newHelper.symbol = symbol;
        encodeHelpers.push_back(newHelper);
    }

    for (DecodeCol dCol : decodeTable)
    {
        char currSymbol = dCol.symbol;
        int symbolIndex = 0;
        while (encodeHelpers[symbolIndex].symbol != currSymbol)
            symbolIndex++;
        encodeHelpers[symbolIndex].states.push_back(dCol.state);
        encodeHelpers[symbolIndex].yVals.push_back(dCol.y);
        encodeHelpers[symbolIndex].kVals.push_back(dCol.k);
        encodeHelpers[symbolIndex].yPrimVals.push_back(dCol.y << dCol.k);
    }
    //  Build the encoding table, one column per state in decoding table
    for (int helperNum = 0; helperNum < encodeHelpers.size(); helperNum++)
    {
        for (int i = 0; i < encodeHelpers[helperNum].states.size(); i++)
        {
            EncodeSymbolData currSymbol;
            currSymbol.symbol = encodeHelpers[helperNum].symbol;
            currSymbol.streamValue = 0;
            currSymbol.numBits = encodeHelpers[helperNum].kVals[i];
            currSymbol.nextState = encodeHelpers[helperNum].states[i];
            encodeTable[encodeHelpers[helperNum].yPrimVals[i]-encodeTable.size()].symbols.push_back(currSymbol);
        }
        EncodeSymbolData currSymbol;
        int prevSize = 0;
        for (int i = 0; i < encodeTable.size(); i++)
        {
            if (i == 0)
            {
                currSymbol = encodeTable[i].symbols[helperNum];
                prevSize = encodeTable[i].symbols.size();
            }
            else if (prevSize < encodeTable[i].symbols.size())
            {
                currSymbol = encodeTable[i].symbols[helperNum];
                prevSize = encodeTable[i].symbols.size();
            }
            else
            {
                prevSize = encodeTable[i].symbols.size();
                currSymbol.streamValue++;
                encodeTable[i].symbols.push_back(currSymbol);
            }
        }
    }

    return encodeTable;
}

EncodedData encodeString(std::string input, std::vector<EncodeCol> encodingTable)
{
    EncodedData data;
    int state;
    //  Reverse the input, since ANS operates in FILO mode
    std::string reverseString = "";
    for (int i = 0; i < input.size(); i++)
    {
        reverseString = input[i] + reverseString;
    }
    //  Encode the string
    int currentOffsetState = 0;
    for (int i = 0; i < reverseString.size(); i++)
    {
        char currChar = reverseString[i];
        //  Find correct encoding instruction for the symbol
        for (EncodeSymbolData eSymbols : encodingTable[currentOffsetState].symbols)
        {
            if (eSymbols.symbol == currChar)
            {
                //  Encode the symbol
                state = eSymbols.nextState;
                if (i == 0)
                    break;
                int streamValue = eSymbols.streamValue;
                int compVal = 1;
                for (int numBit = eSymbols.numBits-1; numBit >= 0; numBit--)
                {
                    bool currBit = (streamValue >> numBit) & compVal;
                    data.bitStream.push_back(currBit);
                }
                break;
            }
        }
        currentOffsetState = state - encodingTable.size();
    }
    data.initialState = state;

    return data;
}

std::string decodeString(EncodedData *data, std::vector<DecodeCol> decodeTable, int numChars)
{
    unsigned int tableSize = decodeTable.size();
    unsigned int state = data->initialState;
    std::string decodedString = "";
    decodedString += decodeTable[state-tableSize].symbol;
    for (int i = 0; i < numChars-1; i++)
    {
        unsigned int currY = decodeTable[state-tableSize].y;
        unsigned int currK = decodeTable[state-tableSize].k;
        unsigned int streamValue = 0;
        //  Horrible way of reading a value from the bitstream
        for (int j = 0; j < currK; j++)
        {
            unsigned int tempVal;
            bool val = data->bitStream.back();
            data->bitStream.pop_back();
            if (val)
                tempVal = 1;
            else
                tempVal = 0;
            streamValue += (tempVal << j);
        }
        //  Calculate the next state and retrieve the symbol for that state
        state = (currY << currK) + streamValue;
        decodedString += decodeTable[state-tableSize].symbol;
    }
    return decodedString;
}

std::vector<char> findSymbols(std::string input)
{
    std::vector<char> symbols;
    for (int i = 0; i < input.size(); i++)
    {
        bool foundSym = false;
        for (char symbol : symbols)
        {
            if (symbol == input[i])
            {
                foundSym = true;
                break;
            }
        }
        if (!foundSym)
            symbols.push_back(input[i]);
    }
    return symbols;
}

std::vector<int> countSymbols(std::string input, std::vector<char> symbols)
{
    std::vector<int> symbolCounts(symbols.size());
    for (int i = 0; i < input.size(); i++)
    {
        char currChar = input[i];
        for (int j = 0; j < symbols.size(); j++)
        {
            if (currChar == symbols[j])
            {
                symbolCounts[j]++;
                break;
            }
        }
    }
    return symbolCounts;
}

std::vector<int> normalizeCounts(std::vector<int> counts, int tableSize)
{
    int totalCount = 0;
    for (int count : counts)
    {
        totalCount += count;
    }
    bool shouldContinue = true;
    std::vector<int> normCounts(counts.size());
    while (shouldContinue)
    {
        int smallestCount;
        int smallestIndex;
        int firstIndex;
        int largestCount = 0;
        for (int i = 0; i < counts.size(); i++)
        {
            if (counts[i] != 0)
            {
                firstIndex = i;
                smallestIndex = i;
                smallestCount = counts[i];
                break;
            }
        }
        for (int i = firstIndex; i < counts.size(); i++)
        {
            if (counts[i] < smallestCount && counts[i] != 0)
            {
                smallestCount = counts[i];
                smallestIndex = i;
            }
            if (counts[i] > largestCount)
            {
                largestCount = counts[i];
            }
        }
        if (largestCount == 0)
        {
            shouldContinue = false;
        }
        else
        {
            float frac = float(smallestCount)/float(totalCount);
            int newCount = std::round(frac*float(tableSize));
            if (newCount == 0 and counts[smallestIndex] != 0)
            {
                newCount = 1;
            }
            normCounts[smallestIndex] = newCount;
            tableSize -= newCount;
            counts[smallestIndex] = 0;
        }
    }
    return normCounts;
}

void printEncodeTable(std::vector<EncodeCol> encodeTable, std::vector<char> symbols)
{
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
}

void printDecodeTable(std::vector<DecodeCol> decodeTable)
{
    for (DecodeCol currCol : decodeTable)
    {
        std::cout << "State: " << currCol.state << " ->Symbol: " << currCol.symbol << " ->y, k " << currCol.y << "," << currCol.k << "\n";
    }
}
