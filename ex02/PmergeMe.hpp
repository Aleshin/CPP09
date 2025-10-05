#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <iostream>
#include <algorithm>

class PmergeMe {
private:
    std::vector<int> _numbers;
    size_t _level;

public:
    PmergeMe();
    explicit PmergeMe(const std::vector<int>& input);
    ~PmergeMe();

    void Insertion(size_t stride);
    void FordJohnson();
    void print(size_t blockSize) const;
};

#endif
