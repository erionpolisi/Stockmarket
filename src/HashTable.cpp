#include "HashTable.h"
#include <fstream>
#include <sstream>
#include <cmath>

#include "functions.h"

HashTable::HashTable() {}

/*DJB2-Algorithmus*/
int HashTable::hashFunction(const std::string& key) const
{
    unsigned int hash = 5381; //ausgewählte Primzahl als Startwert

    for (char c : key)
    {
       hash = ((hash << 5) + hash) + c;// ==     hash = hash * 33 + c;   == Buchstaben werden iteriert und hash wird berechnet
    }

    return hash % TABLE_SIZE; // sorgt dafür, dass der Wert in den Bereich 0 - 1008 fällt

}

int HashTable::quadraticProbing(int hash, int i) const
{
    return (hash + i * i) % TABLE_SIZE;
}

bool HashTable::insert(const Stock& stock)
{
    int hashSymbol = hashFunction(stock.getSymbol()); // Hash für Symbol
    int hashName = hashFunction(stock.getName()); // Hash für Name

    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        int idxSymbol = quadraticProbing(hashSymbol, i);    //Bei Kollision wird hash verschoben
        int idxName = quadraticProbing(hashName, i);

        if (!table[idxSymbol].occupied)
        {
            table[idxSymbol].stock = new Stock(stock);  /*Aktie wird in Hashtabelle mit Kürzel gesetzt*/
            table[idxSymbol].occupied = true;
        }

        if (!nametable[idxName].occupied)
        {
            nametable[idxName].stock = table[idxSymbol].stock; /*Aktie wird in Hashtabelle mit Name gesetzt*/
            nametable[idxName].occupied = true;
            return true;
        }
    }

    message("Adding Went Wrong!");
    return false; // Kein Platz gefunden
}


bool HashTable::remove(const std::string& key)
{
    int hashSymbol = hashFunction(key);
    int hashName = -1;
    bool removed = false;

    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        int idxSymbol = quadraticProbing(hashSymbol, i);

        if (table[idxSymbol].occupied && table[idxSymbol].stock->getSymbol() == key)
        {
            hashName = hashFunction(table[idxSymbol].stock->getName()); // Hash für den Namen berechnen

            delete table[idxSymbol].stock; /*Aktie löschen */
            table[idxSymbol].stock = nullptr;
            table[idxSymbol].occupied = false;
            removed = true;
            break;
        }
    }

    if (!removed)
    {
        message("Removing Went Wrong!");
        return false;
    }

    /* Entferne die Aktie auch aus nametable*/
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        int idxName = quadraticProbing(hashName, i);

        if (nametable[idxName].occupied && nametable[idxName].stock != nullptr)
        {
            nametable[idxName].stock = nullptr; // nametable wird entfernt
            nametable[idxName].occupied = false;
            break;
        }
    }

    message("Removed Successfully!");
    return true;
}


Stock* HashTable::searchBySymbol(const std::string& symbol)
{
    int hash = hashFunction(symbol);

    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        int idx = quadraticProbing(hash, i);
        if (table[idx].occupied && table[idx].stock->getSymbol() == symbol)
            return table[idx].stock;
    }

    return nullptr;
}

Stock* HashTable::searchByName(const std::string& name)
{
    int hash = hashFunction(name);

    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        int idx = quadraticProbing(hash, i);
        if (nametable[idx].occupied && nametable[idx].stock->getName() == name)
            return nametable[idx].stock;
    }

    return nullptr;
}

bool HashTable::saveHashTable(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error saving file: " << filename << std::endl;
        return false;
    }

    for (const auto& entry : table) {
        if (entry.occupied && entry.stock != nullptr) {
            file << entry.stock->getName() << "," << entry.stock->getSymbol() << "\n";

            // Jetzt mit der Getter-Funktion!
            for (const auto& p : entry.stock->getPrices()) {
                file << p.date << "," << p.close << "," << p.volume << "," << p.open
                     << "," << p.high << "," << p.low << "\n";
            }
            file << "#\n"; // Trennzeichen für Aktien
        }
    }
    file.close();
    return true;
}

bool HashTable::loadHashTable(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error loading file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line == "#") continue; // Überspringt Trennzeichen
        std::stringstream ss(line);
        std::string name, symbol;
        std::getline(ss, name, ',');
        std::getline(ss, symbol, ',');

        Stock stock(name, "UNKNOWN_WKN", symbol);
        std::vector<PriceData> prices;

        while (std::getline(file, line) && line != "#") {
            std::stringstream ss(line);
            PriceData data;
            std::string temp;
            std::getline(ss, data.date, ',');
            ss >> data.close; ss.ignore();
            ss >> data.volume; ss.ignore();
            ss >> data.open; ss.ignore();
            ss >> data.high; ss.ignore();
            ss >> data.low;
            prices.push_back(data);
        }

        std::vector<std::tuple<std::string, double, double, double, double, int>> tuplePrices;
        for (const auto& p : prices) {
        tuplePrices.emplace_back(p.date, p.open, p.high, p.low, p.close, p.volume);
        }

        stock.addPriceData(tuplePrices);
        insert(stock);
    }
    file.close();
    return true;
}
