#include "BitcoinExchange.hpp"
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <iomanip>

std::string BitcoinExchange::trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

bool BitcoinExchange::isValidDate(const std::string& d) const {
    if (d.size() != 10 || d[4] != '-' || d[7] != '-') return false;
    for (size_t i = 0; i < d.size(); ++i)
        if (i != 4 && i != 7 && !std::isdigit(static_cast<unsigned char>(d[i])))
            return false;

    int y = std::atoi(d.substr(0, 4).c_str());
    int m = std::atoi(d.substr(5, 2).c_str());
    int day = std::atoi(d.substr(8, 2).c_str());
    if (m < 1 || m > 12) return false;

    static const int dim[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int days = dim[m - 1];
    bool leap = (y % 4 == 0) && (y % 100 != 0 || y % 400 == 0);
    if (m == 2 && leap) days = 29;

    return day >= 1 && day <= days;
}

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const std::string& dbFile) {
    std::ifstream db(dbFile.c_str());
    if (!db.is_open())
        throw std::runtime_error("Error: could not open database file.");

    std::string line;
    std::getline(db, line);
    while (std::getline(db, line)) {
        size_t pos = line.find(',');
        if (pos == std::string::npos) continue;
        std::string date = line.substr(0, pos);
        double price = atof(line.substr(pos + 1).c_str());
        _rates[date] = price;
    }
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _rates(other._rates) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this != &other)
        _rates = other._rates;
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

double BitcoinExchange::getRate(const std::string& date) const {
    std::map<std::string, double>::const_iterator it = _rates.lower_bound(date);
    if (it == _rates.end()) {
        --it;
    } else if (it->first != date) {
        if (it == _rates.begin())
            throw std::runtime_error("Error: date too early.");
        --it;
    }
    return it->second;
}

void BitcoinExchange::processLine(const std::string& line) const {
    if (line.empty())
        return;

    size_t pos = line.find('|');
    if (pos == std::string::npos) {
        std::cerr << "Error: ther is no | separator => " << line << std::endl;
        return;
    }

    std::string date = trim(line.substr(0, pos));
    std::string valueStr = trim(line.substr(pos + 1));

    char* end;
    double value = strtod(valueStr.c_str(), &end);
    if (*end != '\0') {
        std::cerr << "Error: not a number => " << line << std::endl;
        return;
    }
    if (value < 0) {
        std::cerr << "Error: not a positive number." << std::endl;
        return;
    }
    if (value > 1000) {
        std::cerr << "Error: too large a number." << std::endl;
        return;
    }
    if (!isValidDate(date)) {
        std::cerr << "Error: bad date => " << date << std::endl;
        return;
    }

    try {
        double rate = getRate(date);
        std::cout << date << " => " << value << " = "
          << std::fixed << std::setprecision(2) << (value * rate) << std::endl;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
