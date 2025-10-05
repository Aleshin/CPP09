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
    sorter.FordJohnson();
    sorter.Insertion(1);
    sorter.print(1);
    return 0;
}
