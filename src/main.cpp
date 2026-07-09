#include <iostream>
#include <fstream>
#include <vector>
#include "Parser.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <tick_file.csv>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1], std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << argv[1] << std::endl;
        return 1;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        std::cerr << "Failed to read file." << std::endl;
        return 1;
    }

    Parser parser;
    Quote quote;
    int parsedCount = 0;

    std::string_view data(buffer.data(), size);
    size_t start = 0;
    while (start < data.size()) {
        size_t end = data.find('\n', start);
        if (end == std::string_view::npos) end = data.size();
        
        std::string_view line = data.substr(start, end - start);
        if (parser.parseLine(line, quote)) {
            parsedCount++;
        }
        start = end + 1;
    }

    std::cout << "Successfully parsed " << parsedCount << " valid quotes using zero-copy buffer." << std::endl;
    return 0;
}

