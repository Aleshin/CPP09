#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <iostream>
#include <algorithm>
#include <sys/time.h>

class PmergeMe {
private:
    std::vector<int> _numbers;
    std::deque<int> _numbersDeq;
    size_t _blockSize;

public:
    PmergeMe();
    explicit PmergeMe(const std::vector<int>& input);
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    void InsertionVec(size_t stride);
    void InsertionDeq(size_t stride);
    void FordJohnsonVec();
    void FordJohnsonDeq();
    void printNums() const;
    void print(size_t blockSize) const;
    void printDeq(size_t blockSize) const;
	static double getCurrentTime() {
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec * 1000000.0 + tv.tv_usec;
    }

};

#endif
