#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <iostream>
#include <algorithm>

class PmergeMe {
private:
    std::vector<int> _numbers;
    size_t _blockSize;

public:
    PmergeMe();
    explicit PmergeMe(const std::vector<int>& input);
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    void Insertion(size_t stride);
    void FordJohnson();
    void print(size_t blockSize) const;
};

#endif
