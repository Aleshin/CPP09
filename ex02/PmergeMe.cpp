#include "PmergeMe.hpp"
#include <iomanip>
#include <algorithm>

PmergeMe::PmergeMe() : _level(0) {}

PmergeMe::PmergeMe(const std::vector<int>& input)
    : _numbers(input), _level(0) {}

PmergeMe::~PmergeMe() {}

void PmergeMe::FordJohnson() {
    size_t n = _numbers.size();
    size_t stride = 1ull << _level;
    size_t step   = stride * 2;

    if (step > n) return;

    for (size_t i = 0; i + stride < n; i += step) {
        size_t a1 = i + stride - 1;
        size_t a2 = std::min(i + step - 1, n - 1);
        if (_numbers[a1] > _numbers[a2]) {
            for (size_t k = 0; k < stride && i + stride + k < n; ++k)
                std::swap(_numbers[i + k], _numbers[i + stride + k]);
        }
    }
    PmergeMe::print(stride*2);
    ++_level;
    FordJohnson();
    --_level;
    Insertion(stride*2);
    PmergeMe::print(stride*2);
}

void PmergeMe::Insertion(size_t block)
{
    const size_t n     = _numbers.size();

    if (block == 0 || n < block * 2)
        return;

    const size_t full_blocks = n / block;

    for (size_t j = 1; j < full_blocks; ++j)
    {
        const size_t cur_start     = j * block;
        const size_t cur_last_idx  = cur_start + block - 1;
        const int    key           = _numbers[cur_last_idx];

        size_t pos = j;
        while (pos > 0)
        {
            const size_t prev_last_idx = pos * block - 1;
            if (_numbers[prev_last_idx] <= key)
                break;
            --pos;
        }

        if (pos != j)
        {
            std::rotate(_numbers.begin() + pos * block,
                        _numbers.begin() + cur_start,
                        _numbers.begin() + cur_start + block);
        }
    }
}

void PmergeMe::print(size_t blockSize) const
{
    const size_t n = _numbers.size();

    std::cout << "[level " << _level << ", block " << blockSize << "]  ";

    for (size_t i = 0; i < n; i += blockSize)
    {
        std::cout << "[";

        size_t blockEnd = std::min(i + blockSize, n);
        for (size_t j = i; j < blockEnd; ++j)
        {
            std::cout << _numbers[j];
            if (j + 1 < blockEnd)
                std::cout << " ";
        }

        std::cout << "]";
        if (i + blockSize < n)
            std::cout << " ";
    }

    std::cout << "\n";
}
