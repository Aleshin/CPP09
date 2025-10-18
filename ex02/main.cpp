#include "PmergeMe.hpp"
#include <cstdlib>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <list of integers>\n";
        return 1;
    }

    std::vector<int> input;
    input.reserve(argc - 1);

    for (int i = 1; i < argc; ++i) {
        if (std::atoi(argv[i]) < 0) {
            std::cerr << "Error: " << argv[i] << "\n";
            return 1;
        }
        input.push_back(std::atoi(argv[i]));
    }

    PmergeMe sorter(input);
    std::cout << "Before:";
    sorter.printNums();
	double start = PmergeMe::getCurrentTime();
    sorter.FordJohnsonVec();
    sorter.InsertionVec(1);
    double end = PmergeMe::getCurrentTime();
    std::cout << "\nAfter:";
    sorter.printNums();
    std::cout << "\nTime to process a range of " << input.size() << " elements with std::vector: " << (end - start) / 1000.0 << " ms\n";
	start = PmergeMe::getCurrentTime();
    sorter.FordJohnsonDeq();
    sorter.InsertionDeq(1);
    end = PmergeMe::getCurrentTime();
    std::cout << "Time to process a range of " << input.size() << " elements with std::deque: " << (end - start) / 1000.0 << " ms\n";
/*
    sorter.print(1);
    sorter.FordJohnsonVec();
    sorter.InsertionVec(1);
    sorter.print(1);
*/
    return 0;
}
//Linux: ./PmergeMe `shuf -i 1-100000 -n 3000 | tr "\n" " "`
//macOS: ./PmergeMe `jot -r 3000 1 100000 | tr '\n' ' '`