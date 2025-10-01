#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <string>

class PmergeMe {
private:
    std::vector<int> _numbers;

public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    void loadInput(int argc, char** argv);
    void printBefore() const;
    void printAfter() const;
};

#endif
