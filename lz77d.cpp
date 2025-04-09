#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdint>

void readMatch(std::ifstream& inputFile, int& matchOffset, int& matchLength, char& nextChar, size_t searchBufSize, size_t lookaheadBufSize) {
    int positionBits = static_cast<int>(std::ceil(std::log2(searchBufSize)));
    int lengthBits = static_cast<int>(std::ceil(std::log2(lookaheadBufSize + 1)));

    int positionBytes = (positionBits + 7) / 8;
    int lengthBytes = (lengthBits + 7) / 8;

    uint32_t positionPart = 0;
    uint32_t lengthPart = 0;

    inputFile.read(reinterpret_cast<char*>(&positionPart), positionBytes);
    inputFile.read(reinterpret_cast<char*>(&lengthPart), lengthBytes);
    inputFile.read(&nextChar, sizeof(nextChar));

    matchOffset = static_cast<int>(positionPart);
    matchLength = static_cast<int>(lengthPart);
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

    std::vector<char> decompressedData;

    while (inputFile.peek() != EOF) {
        int matchOffset = 0, matchLength = 0;
        char nextChar = 0;

        readMatch(inputFile, matchOffset, matchLength, nextChar, searchBufSize, lookaheadBufSize);

        size_t start = decompressedData.size() - matchOffset;
        for (int i = 0; i < matchLength; ++i) {
            decompressedData.push_back(decompressedData[start + i]);
        }

        decompressedData.push_back(nextChar);
    }

    outputFile.write(decompressedData.data(), decompressedData.size());

    inputFile.close();
    outputFile.close();

    return 0;
}
