#include "PmergeMe.hpp"
#include <iomanip>
#include <algorithm>

PmergeMe::PmergeMe() : _blockSize(1) {}

PmergeMe::PmergeMe(const std::vector<int>& input)
    : _numbers(input), _blockSize(1) {}

PmergeMe::PmergeMe(const PmergeMe& other)
    : _numbers(other._numbers), _blockSize(other._blockSize) {
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        _numbers   = other._numbers;
        _blockSize = other._blockSize;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

void PmergeMe::FordJohnson() {
    size_t n = _numbers.size();
    size_t step   = _blockSize * 2;

    if (step > n) return;

    for (size_t i = 0; i + _blockSize < n; i += step) {
        size_t a1 = i + _blockSize - 1;
        size_t a2 = std::min(i + step - 1, n - 1);
        if (_numbers[a1] > _numbers[a2]) {
            for (size_t k = 0; k < _blockSize && i + _blockSize + k < n; ++k)
                std::swap(_numbers[i + k], _numbers[i + _blockSize + k]);
        }
    }
    PmergeMe::print(step);
    _blockSize *=2;
    FordJohnson();
    _blockSize /=2;
    Insertion(step);
    PmergeMe::print(step);
}

static std::vector<std::size_t> buildJacobsthalOrder(std::size_t m) {
    std::vector<std::size_t> order;
    if (m <= 1) return order;
    order.push_back(1);
    std::size_t target = m - 1, prev = 1, j0 = 1, j1 = 1;
    while (prev < target) {
        std::size_t curr = (j1 < target) ? j1 : target;
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
    for (std::size_t i = block; i < src.size(); i += 2 * block)
        M.insert(M.end(), src.begin() + i, src.begin() + i + block);
    return M;
}
// Inserts B-blocks (size = block) into the main chain using Jacobsthal order.
// Uses buildMainChain(_numbers, block) and buildJacobsthalOrder(m).
void PmergeMe::Insertion(std::size_t block)
{
    const std::vector<int>& src = _numbers;
    const std::size_t n = src.size();
    if (block == 0 || n < block * 2) return;

    const std::size_t full_blocks = n / block;        // count of full blocks
    const std::size_t m = full_blocks / 2;            // number of pairs (B_i,A_i)
    std::vector<int> M = buildMainChain(src, block);  // M = B0 A0 A1 A2 ...

    if (m <= 1) { _numbers.swap(M); return; }

    const std::vector<std::size_t> order = buildJacobsthalOrder(m); // indices i in [1..m-1]

    for (std::size_t k = 0; k < order.size(); ++k) {
        const std::size_t i = order[k];

        // --- locate A_i block START in M (by full-block equality with src's A_i) ---
        const std::size_t aStartSrc = (2 * i + 1) * block;
        std::size_t aPos = 0; bool aFound = false;
        for (std::size_t pos = 0; pos + block <= M.size(); pos += block) {
            bool eq = true;
            for (std::size_t t = 0; t < block; ++t)
                if (M[pos + t] != src[aStartSrc + t]) { eq = false; break; }
            if (eq) { aPos = pos; aFound = true; break; }
        }
        if (!aFound) continue; // safety guard

        // --- build list of A-keys (last elements) and their positions BEFORE A_i in M ---
        std::vector<int> aKeys;
        std::vector<std::size_t> aPosBlocks; // positions measured in blocks
        for (std::size_t pos = 0; pos < aPos; pos += block) {
            // test if M[pos..pos+block) equals any A_j from src
            bool isA = false;
            for (std::size_t j = 0; j < m && !isA; ++j) {
                const std::size_t aj = (2 * j + 1) * block;
                bool eq = true;
                for (std::size_t t = 0; t < block; ++t)
                    if (M[pos + t] != src[aj + t]) { eq = false; break; }
                if (eq) isA = true;
            }
            if (isA) {
                aKeys.push_back(M[pos + block - 1]);
                aPosBlocks.push_back(pos / block);
            }
        }

        // --- lower_bound by B_i key among A-keys BEFORE A_i ---
        const std::size_t bStartSrc = (2 * i) * block;
        const int bKey = src[bStartSrc + block - 1];
        std::size_t insIdx = 0;
        while (insIdx < aKeys.size() && aKeys[insIdx] < bKey) ++insIdx;

        // block-position in M where to insert B_i
        const std::size_t insertBlockPos =
            (insIdx < aPosBlocks.size()) ? aPosBlocks[insIdx] : (aPos / block);

        // --- insert whole B_i block from src into M ---
        M.insert(M.begin() + insertBlockPos * block,
                 src.begin() + bStartSrc, src.begin() + bStartSrc + block);
    }

    // move result back
    _numbers.swap(M);
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
