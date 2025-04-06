#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Stock.h"

#include <iostream>
#include <vector>
#include <string>
#include <tuple>

class HashTable {
private:
    struct HashEntry {
        Stock* stock; // Pointer auf das Aktienobjekt
        bool occupied; // Gibt an, ob der Slot belegt ist
        HashEntry() : stock(nullptr), occupied(false) {} //Konstruktor
    };

    static const int TABLE_SIZE = 1009; // Primzahl für Hashtabelle
    HashEntry table[TABLE_SIZE]; // Array für die Hashtabelle mit Kürzel
    HashEntry nametable[TABLE_SIZE]; // Array für die Hashtabelle mit Name

    int hashFunction(const std::string& key) const; // Berechnet den Hashwert mit DJB2-Algorithmus
    int quadraticProbing(int hash, int i) const; // Kollisionsbehandlung

public:
    HashTable(); // Konstruktor
    bool insert(const Stock& stock);
    bool remove(const std::string& key);
    Stock* searchBySymbol(const std::string& key); // Aktie mit Kürzel suchen
    Stock* searchByName(const std::string& key); // Aktie mit Name suchen
    bool saveHashTable(const std::string& filename) const;
    bool loadHashTable(const std::string& filename);
};

#endif // HASHTABLE_H
