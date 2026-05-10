#include <iostream>
#include <fstream>
#include "Parser.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <tick_file.csv>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << argv[1] << std::endl;
        return 1;
    }

    Parser parser;
    std::string line;
    Quote quote;
    int parsedCount = 0;

    while (std::getline(file, line)) {
        if (parser.parseLine(line, quote)) {
            parsedCount++;
        }
    }

    std::cout << "Successfully parsed " << parsedCount << " valid quotes." << std::endl;
    return 0;
}
