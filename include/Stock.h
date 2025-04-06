#ifndef STOCK_H
#define STOCK_H

#include <iostream>
#include <vector>
#include <string>
#include <tuple>

// Struktur zur Speicherung von Kursdaten
struct PriceData {
    std::string date;
    double open, high, low, close;
    int volume;
};

// Klasse zur Verwaltung einzelner Aktien
class Stock {
private:
    std::string name;
    std::string wkn;
    std::string symbol;
    std::vector<PriceData> prices; // Speicherung der letzten 30 Kursdaten

public:
    Stock(const std::string& name, const std::string& wkn, const std::string& symbol);
    void addPriceData(const std::vector<std::tuple<std::string, double, double, double, double, int>>& rawData);
    void printLatestPrice() const;
    void plotPrices() const;
    std::string getName() const;
    std::string getSymbol() const;
    const std::vector<PriceData>& getPrices() const;

    bool importPriceData(const std::string& filename); // Methode zum Importieren von CSV-Daten
};

#endif // STOCK_H
