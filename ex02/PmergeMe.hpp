#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <iostream>
#include <algorithm>
#include <sys/time.h>

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
	static double getCurrentTime() {
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec * 1000000.0 + tv.tv_usec;
    }

};

#endif
