#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <climits>
#include <climits>
#include <climits>

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe& other) : _numbers(other._numbers) {}
PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other)
        _numbers = other._numbers;
    return *this;
}
PmergeMe::~PmergeMe() {}

void PmergeMe::loadInput(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        std::string token(argv[i]);
        for (size_t j = 0; j < token.size(); j++) {
            if (!isdigit(token[j]))
                throw std::runtime_error("Invalid input");
        }
        long val = std::strtol(token.c_str(), NULL, 10);
        if (val <= 0 || val > INT_MAX)
            throw std::runtime_error("Invalid number");
        _numbers.push_back(static_cast<int>(val));
    }
}

void PmergeMe::printBefore() const {
    std::cout << "Before: ";
    for (size_t i = 0; i < _numbers.size(); i++)
        std::cout << _numbers[i] << " ";
    std::cout << std::endl;
}

void PmergeMe::printAfter() const {
    std::cout << "After:  ";
    for (size_t i = 0; i < _numbers.size(); i++)
        std::cout << _numbers[i] << " ";
    std::cout << std::endl;
}
