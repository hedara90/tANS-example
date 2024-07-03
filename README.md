# tANS example implementation

This is a simple example implementation of tANS encoding.

## How to use

1. Create a `std::string` with the data to be encoded.
2. Create a `std::vector<char>` with all unique symbols in the string.
3. Create a `std::vector<int>` with the frequencies of occurences for the symbols, normalized to a power of 2.
  * Note: Frequencies cannot be 0. Program may behave unexpectedly then.
3. Call `std::vector<DecodeCol> decodeTable = createDecodingTable(symbols, frequencies)`.
4. Call `std::vector<EncodeCol> encodeTable = createEncodingTable(decodeTable, symbols)`.
5. Do your operations.
  * `EncodedData data = encodeString(stringToEncode, encodeTable)` for encoding.
  * `std::string decodedString = decodeString(&data, decodeTable, numCharsToDecode)` to decode `numCharsToDecode` from `data`.
