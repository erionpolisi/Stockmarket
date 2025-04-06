#include "Stock.h"
#include <fstream>
#include <sstream>
#include <iostream>

Stock::Stock(const std::string& name, const std::string& wkn, const std::string& symbol)
    : name(name), wkn(wkn), symbol(symbol) {}

void Stock::addPriceData(const std::vector<std::tuple<std::string, double, double, double, double, int>>& rawData) {
    prices.clear();
    for (const auto& entry : rawData) {
        prices.push_back({
            std::get<0>(entry), std::get<1>(entry), std::get<2>(entry),
            std::get<3>(entry), std::get<4>(entry), std::get<5>(entry)
        });
    }
}

bool Stock::importPriceData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    std::string line;
    std::getline(file, line); // Skip header
    std::vector<PriceData> stockRecords;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        PriceData data;
        std::string temp;

        std::getline(ss, data.date, ',');
        std::getline(ss, temp, ',');
        data.close = std::stod(temp.substr(1));
        std::getline(ss, temp, ',');
        data.volume = std::stol(temp);
        std::getline(ss, temp, ',');
        data.open = std::stod(temp.substr(1));
        std::getline(ss, temp, ',');
        data.high = std::stod(temp.substr(1));
        std::getline(ss, temp, ',');
        data.low = std::stod(temp.substr(1));

        stockRecords.push_back(data);
    }

    file.close();
    prices = stockRecords;
    return true;
}

void Stock::printLatestPrice() const {
    if (prices.empty()) {
        std::cout << "No stock data available for " << name << std::endl;
        return;
    }
    const PriceData& latest = prices.back();
    std::cout << "\nLatest stock price for " << name << " (" << symbol << ") on " << latest.date << ":\n"
              << "Open: " << latest.open << " | High: " << latest.high
              << " | Low: " << latest.low << " | Close: " << latest.close
              << " | Volume: " << latest.volume << std::endl;
}

void Stock::plotPrices() const {
    if (prices.empty()) {
        std::cout << "No stock data available." << std::endl;
        return;
    }
    for (const auto& p : prices) {
        int numStars = static_cast<int>(p.close / 10);
        std::cout << p.date << " | " << std::string(numStars, '*') << " (" << p.close << ")" << std::endl;
    }
}

std::string Stock::getName() const { return name; }
std::string Stock::getSymbol() const { return symbol; }
const std::vector<PriceData>& Stock::getPrices() const { return prices; }

