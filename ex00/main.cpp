#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Error: wrong arguments. Use ./btc filename" << std::endl;
        return 1;
    }

    try {
        BitcoinExchange btc("data.csv");
        std::ifstream input(argv[1]);
        if (!input.is_open()) {
            std::cerr << "Error: could not open file." << std::endl;
            return 1;
        }

        std::string line;
        std::getline(input, line);
        while (std::getline(input, line)) {
            btc.processLine(line);
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
