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
        input.push_back(std::atoi(argv[i]));
    }

    PmergeMe sorter(input);

    sorter.print(1);
	double start = PmergeMe::getCurrentTime();
    sorter.FordJohnson();
    sorter.Insertion(1);
    sorter.print(1);
    double end = PmergeMe::getCurrentTime();
    std::cout << "Time to process a range of " << input.size() << " elements with std::vector: " << (end - start) / 1000.0 << " ms\n";
    return 0;
}
//Linux: ./PmergeMe `shuf -i 1-100000 -n 3000 | tr "\n" " "`
//macOS: ./PmergeMe `jot -r 3000 1 100000 | tr '\n' ' '`