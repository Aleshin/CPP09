#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <iostream>

class BitcoinExchange {
private:
    std::map<std::string, double> _rates;

    static std::string trim(const std::string& s);
    bool isValidDate(const std::string& date) const;
    double getRate(const std::string& date) const;

public:
    BitcoinExchange();
    BitcoinExchange(const std::string& dbFile);
    BitcoinExchange(const BitcoinExchange& other);
    BitcoinExchange& operator=(const BitcoinExchange& other);
    ~BitcoinExchange();

    void processLine(const std::string& line) const;
};
#endif
