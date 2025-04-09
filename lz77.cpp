#include <algorithm>
#include <iostream>
#include <cstring>
#include <tuple>
#include <fstream>
#include <vector>
#include <span>
#include <bitset>
#include <cmath>

constexpr size_t SEARCH_BUF_SIZE = 4095;
constexpr size_t LOOKAHEAD_BUF_SIZE = 15;

std::tuple<int, int> findLongestMatch(const char* inputBuf,
                                      size_t searchBufOffset,
                                      size_t searchBufLength,
                                      size_t lookaheadBufOffset,
                                      size_t lookaheadBufLength) {
    int longestMatchLength = 0;
    int longestMatchPosition = 0;

    for (size_t searchBufIndex = 0; searchBufIndex < searchBufLength; ++searchBufIndex) {
        size_t lookaheadBufIndex = 0; // Changed to size_t

        while (lookaheadBufIndex < lookaheadBufLength) {
            if (inputBuf[searchBufOffset + searchBufIndex + lookaheadBufIndex] !=
                inputBuf[lookaheadBufOffset + lookaheadBufIndex]) {
                break;
            }
            ++lookaheadBufIndex;
        }

        if (lookaheadBufIndex > static_cast<size_t>(longestMatchLength)) {
            longestMatchLength = static_cast<int>(lookaheadBufIndex);
            longestMatchPosition = static_cast<int>(searchBufLength - searchBufIndex);
        }
    }

    return {longestMatchPosition, longestMatchLength};
}


void writeMatch(std::ofstream& outputFile, const char* inputBuf, size_t lookaheadBufOffset, int matchOffset, int matchLength, size_t searchBufSize, size_t lookaheadBufSize) {
    int positionBits = static_cast<int>(std::ceil(std::log2(searchBufSize)));
    int lengthBits = static_cast<int>(std::ceil(std::log2(lookaheadBufSize + 1))); // +1 for zero-length matches

    int positionBytes = (positionBits + 7) / 8;
    int lengthBytes = (lengthBits + 7) / 8;

    uint8_t nextChar = static_cast<uint8_t>(inputBuf[lookaheadBufOffset + matchLength]);
    outputFile.write(reinterpret_cast<const char*>(&matchOffset), positionBytes);
    outputFile.write(reinterpret_cast<const char*>(&matchLength), lengthBytes);
    outputFile.write(reinterpret_cast<const char*>(&nextChar), sizeof(nextChar));
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <search_buf_size> <lookahead_buf_size>" << std::endl;
        return 1;
    }

    std::string inputFilePath = argv[1];
    std::string outputFilePath = argv[2];
    size_t searchBufSize = std::stoul(argv[3]);
    size_t lookaheadBufSize = std::stoul(argv[4]);

    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1;
    }

    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Error: Could not open output file." << std::endl;
        return 1;
    }

    inputFile.seekg(0, std::ios::end);
    std::streamsize inputSizeSigned = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    if (inputSizeSigned <= 0) {
        std::cerr << "Error: Input file is empty or invalid." << std::endl;
        return 1;
    }

    size_t inputSize = static_cast<size_t>(inputSizeSigned);

    char* inputBuf = new char[inputSize];
    if (!inputFile.read(inputBuf, inputSize)) {
        std::cerr << "Error: Could not read input file into buf." << std::endl;
        delete[] inputBuf;
        return 1;
    }

    size_t searchBufOffset{};
    size_t searchBufLength{};

    size_t lookaheadBufOffset{};
    size_t lookaheadBufLength{};

    while (lookaheadBufOffset < inputSize) { // No need to cast inputSize here
        searchBufLength = std::min(searchBufSize, lookaheadBufOffset);
        searchBufOffset = std::max(lookaheadBufOffset - searchBufLength, static_cast<size_t>(0));

        lookaheadBufLength = std::min(lookaheadBufSize, inputSize - lookaheadBufOffset);

        auto [matchOffset, matchLength] = findLongestMatch(inputBuf, searchBufOffset, searchBufLength, lookaheadBufOffset, lookaheadBufLength);

        writeMatch(outputFile, inputBuf, lookaheadBufOffset, matchOffset, matchLength, searchBufSize, lookaheadBufSize);
        lookaheadBufOffset += matchLength + 1;

    delete[] inputBuf;
    inputFile.close();
    outputFile.close();

    return 0;
}
