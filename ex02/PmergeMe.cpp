#include "PmergeMe.hpp"
#include <iomanip>
#include <algorithm>

PmergeMe::PmergeMe()
    : _numbers(), _numbersDeq(), _blockSize(1) {}

PmergeMe::PmergeMe(const std::vector<int>& input)
    : _numbers(input),
      _numbersDeq(input.begin(), input.end()),
      _blockSize(1) {}

PmergeMe::PmergeMe(const PmergeMe& other)
    : _numbers(other._numbers),
      _numbersDeq(other._numbersDeq),
      _blockSize(other._blockSize) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        _numbers    = other._numbers;
        _numbersDeq = other._numbersDeq;
        _blockSize  = other._blockSize;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

void PmergeMe::FordJohnsonVec() {
    size_t n = _numbers.size();
    size_t step   = _blockSize * 2;

    if (step > n) return;

    for (size_t i = 0; i + step <= n; i += step) {
        const size_t a1 = i + _blockSize - 1;
        const size_t a2 = i + step - 1;
        if (_numbers[a1] > _numbers[a2]) {
            for (size_t k = 0; k < _blockSize; ++k)
                std::swap(_numbers[i + k], _numbers[i + _blockSize + k]);
        }
    }
//    PmergeMe::print(step);
    _blockSize *=2;
    FordJohnsonVec();
    _blockSize /=2;
    InsertionVec(step);
//    PmergeMe::print(step);
}

void PmergeMe::FordJohnsonDeq() {
    std::size_t n    = _numbersDeq.size();
    std::size_t step = _blockSize * 2;

    if (step > n) return;

    for (std::size_t i = 0; i + step <= n; i += step) {
        const std::size_t a1 = i + _blockSize - 1;
        const std::size_t a2 = i + step - 1;
        if (_numbersDeq[a1] > _numbersDeq[a2]) {
            for (std::size_t k = 0; k < _blockSize; ++k)
                std::swap(_numbersDeq[i + k], _numbersDeq[i + _blockSize + k]);
        }
    }

    _blockSize *= 2;
    FordJohnsonDeq();
    _blockSize /= 2;

    InsertionDeq(step);
}

static std::vector<std::size_t> buildJacobsthalOrder(std::size_t m) {
    std::vector<std::size_t> order;
    if (m <= 1) return order;
    order.push_back(1);
    std::size_t prev = 1, j0 = 1, j1 = 1;
    while (prev < m - 1) {
        std::size_t curr = (j1 < (m - 1)) ? j1 : (m - 1);
        for (std::size_t i = curr; i > prev; --i) order.push_back(i);
        prev = curr;
        std::size_t next = j1 + 2 * j0; j0 = j1; j1 = next;
    }
    return order;
}

static std::vector<int> buildMainChain(const std::vector<int>& src, std::size_t block) {
    std::vector<int> M;
    if (src.size() < block) return M;
    M.insert(M.end(), src.begin(), src.begin() + block); // add b0 to the 1st pos
    for (std::size_t i = block; i + block <= src.size(); i += 2 * block)
        M.insert(M.end(), src.begin() + i, src.begin() + i + block);
    return M;
}

void PmergeMe::InsertionVec(std::size_t block)
{
    const std::size_t n = _numbers.size();
    if (block == 0 || n < block * 2) return;

    const std::size_t full_blocks = n / block; // count of full blocks
    const std::size_t m = full_blocks / 2 + full_blocks % 2; // number of B include last one
    std::vector<int> MainChain = buildMainChain(_numbers, block);  // M = B0 A0 A1 A2 ...

    if (m <= 1) {
    // append tail (< block) if any
        if (full_blocks * block < n)
            MainChain.insert(MainChain.end(),
                             _numbers.begin() + full_blocks * block,
                             _numbers.end());
    _numbers.swap(MainChain);
    return;
    }

    const std::vector<std::size_t> JacobsthalOrder = buildJacobsthalOrder(m); // indices i in [1..m-1]

    for (std::size_t k = 0; k < JacobsthalOrder.size(); ++k) {
        const std::size_t i = JacobsthalOrder[k];

        // take Bi block
        const std::size_t bStart = (2*i)*block;
        const int bKey = _numbers[bStart + block - 1];

        // locate Ai block start in MainChain
        const std::size_t aStart = (2*i + 1)*block;
        std::size_t aPos = 0; bool aFound = false;
        if (aStart + block > n) {
            aPos = MainChain.size();
            aFound = true;
        } else {
                for (std::size_t p = 0; p + block <= MainChain.size(); p += block) {
                    bool eq = true;
                    for (std::size_t t = 0; t < block; ++t)
                        if (MainChain[p + t] != _numbers[aStart + t]) { eq = false; break; }
                    if (eq) { aPos = p; aFound = true; break; }
                }
        }
        if (!aFound) continue;

        // binary search over ALL blocks before Ai
        std::vector<int> keys; keys.reserve(aPos / block);
        for (std::size_t p = 0; p < aPos; p += block)
            keys.push_back(MainChain[p + block - 1]);
        const std::size_t insBlock =
            std::lower_bound(keys.begin(), keys.end(), bKey) - keys.begin();

        // insert Bi before Ai at the found block position
        MainChain.insert(MainChain.begin() + insBlock * block,
                         _numbers.begin() + bStart,
                         _numbers.begin() + bStart + block);
    }
    if (full_blocks * block < n)
    MainChain.insert(MainChain.end(),
                     _numbers.begin() + full_blocks * block,
                     _numbers.end());

    // move result back
    _numbers.swap(MainChain);
}

void PmergeMe::InsertionDeq(std::size_t block)
{
    const std::size_t n = _numbersDeq.size();
    if (block == 0 || n < block * 2) return;

    const std::size_t full_blocks = n / block;                    // count of full blocks
    const std::size_t m = full_blocks / 2 + full_blocks % 2;      // number of B (include lonely last)

    // Build main chain in-place for deque: M = B0 A0 A1 A2 ...
    std::deque<int> MainChain;
    if (n >= block) {
        MainChain.insert(MainChain.end(), _numbersDeq.begin(), _numbersDeq.begin() + block); // B0
        for (std::size_t i = block; i + block <= n; i += 2 * block)                           // A-blocks only if full
            MainChain.insert(MainChain.end(), _numbersDeq.begin() + i, _numbersDeq.begin() + i + block);
    }

    if (m <= 1) {
        // append tail (< block) if any
        if (full_blocks * block < n)
            MainChain.insert(MainChain.end(),
                             _numbersDeq.begin() + full_blocks * block,
                             _numbersDeq.end());
        _numbersDeq.swap(MainChain);
        return;
    }

    const std::vector<std::size_t> JacobsthalOrder = buildJacobsthalOrder(m); // indices i in [1..m-1]

    for (std::size_t k = 0; k < JacobsthalOrder.size(); ++k) {
        const std::size_t i = JacobsthalOrder[k];

        // take Bi block
        const std::size_t bStart = (2 * i) * block;
        const int bKey = _numbersDeq[bStart + block - 1];

        // locate Ai block start in MainChain
        const std::size_t aStart = (2 * i + 1) * block;
        std::size_t aPos = 0; bool aFound = false;

        if (aStart + block > n) { // lonely trailing B: search over whole MainChain
            aPos = MainChain.size();
            aFound = true;
        } else {
            for (std::size_t p = 0; p + block <= MainChain.size(); p += block) {
                bool eq = true;
                for (std::size_t t = 0; t < block; ++t)
                    if (MainChain[p + t] != _numbersDeq[aStart + t]) { eq = false; break; }
                if (eq) { aPos = p; aFound = true; break; }
            }
        }
        if (!aFound) continue;

        // binary search over ALL blocks before Ai (by last element/key of each block)
        std::vector<int> keys; keys.reserve(aPos / block);
        for (std::size_t p = 0; p < aPos; p += block)
            keys.push_back(MainChain[p + block - 1]);

        const std::size_t insBlock =
            std::lower_bound(keys.begin(), keys.end(), bKey) - keys.begin();

        // insert Bi before Ai at the found block position
        MainChain.insert(MainChain.begin() + insBlock * block,
                         _numbersDeq.begin() + bStart,
                         _numbersDeq.begin() + bStart + block);
    }

    // append tail (< block) if any
    if (full_blocks * block < n)
        MainChain.insert(MainChain.end(),
                         _numbersDeq.begin() + full_blocks * block,
                         _numbersDeq.end());

    _numbersDeq.swap(MainChain);
}

void PmergeMe::print(size_t blockSize) const
{
    const size_t n = _numbers.size();
    std::cout << "block " << blockSize << ": ";
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

void PmergeMe::printNums() const {
    for (std::size_t i = 0; i < _numbers.size(); i++) {
        std::cout << " " << _numbers[i];        
    }
}
