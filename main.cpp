#include "Stock.h"
#include "HashTable.h"
#include "functions.h" //Für Verschönerung des Codes

#include <iostream>
#include <limits>

int main() {
    HashTable stockTable;
    int choice;
    int exit;
    do {
        system("cls"); //Für Windows

        std::cout << "\nMenu Options:" << std::endl;
        std::cout << "1. ADD: Add a stock (Name, WKN, Symbol)" << std::endl;
        std::cout << "2. DEL: Delete a stock" << std::endl;
        std::cout << "3. IMPORT: Import stock price data from a CSV file" << std::endl;
        std::cout << "4. SEARCH: Search for a stock by name or symbol" << std::endl;
        std::cout << "5. PLOT: Display closing prices as ASCII graphics" << std::endl;
        std::cout << "6. SAVE <filename>: Save the hashtable to a file" << std::endl;
        std::cout << "7. LOAD <filename>: Load the hashtable from a file" << std::endl;
        std::cout << "8. QUIT: Exit the program" << std::endl;

        std::cout << "Select an option: ";
        std::string input;
        std::getline(std::cin >> std::ws, input);

        try {
            choice = std::stoi(input);
        } catch (std::invalid_argument&) {
            message("Invalid Option");
            continue;
        }

        if (choice == 1) {
            std::string name, wkn, symbol;
            std::cout << "Name: "; std::cin >> name;
            std::cout << "WKN: "; std::cin >> wkn;
            std::cout << "Symbol: "; std::cin >> symbol;

            void clearScreen();
            Stock newStock(name, wkn, symbol);
            if ( stockTable.insert(newStock) ) message("Added Succesfully!");
        }
        else if (choice == 2) {
            std::string symbol;
            std::cout << "Stock symbol: "; std::cin >> symbol;
            stockTable.remove(symbol);
        }
        else if (choice == 3) {
            std::string symbol;
            std::cout << "Enter Stock Symbol: "; std::cin >> symbol;

            Stock* foundStock = stockTable.searchBySymbol(symbol);
            if (foundStock) {
                std::string filename = symbol + ".csv";
                if (foundStock->importPriceData(filename)) {
                    message("Stock data imported successfully!"); //in functions.h
                } else {
                    message("Failed to import stock data!");
                }
            } else {
                message("Stock not found!");
            }
        }
        else if (choice == 4) {
            std::string input;
            std::cout << "Enter Stock Symbol or Name: ";
            std::cin >> input;

            Stock* foundStock = stockTable.searchBySymbol(input);
            if (!foundStock) {
                foundStock = stockTable.searchByName(input);
            }

            if (foundStock == nullptr) { //Sicherstellen, dass nicht auf nullptr zugegriffen wird
                message("Stock not found!");
                continue;
            }

            foundStock->printLatestPrice(); // Nur aufrufen, wenn foundStock existiert

            while (1) {
                std::cout << "Type 1 to exit: ";
                std::cin >> exit;
                if (exit == 1) break;
            }
        }
        else if (choice == 5) {
            std::string symbol;

            std::cout << "Stock symbol: "; std::cin >> symbol;
            Stock* foundStock = stockTable.searchBySymbol(symbol);
            if (foundStock) {
                foundStock->plotPrices();
            } else {
                message("Stock not found!");
                continue;
            }

            while(1)
            {
                std::cout << "type 1 to exit:" << std::endl;
                std::cin >> exit;
                if(exit == 1) break;
            }
        }
        else if (choice == 6) {
            std::string filename;
            std::cout << "Enter filename to save: ";
            std::cin >> filename;
            if (stockTable.saveHashTable(filename)) {
                message("Hashtable saved successfully!");
            } else {
                message("Error saving hashtable!");
            }
        }
        else if (choice == 7) {
            std::string filename;
            std::cout << "Enter filename to load: ";
            std::cin >> filename;
            if (stockTable.loadHashTable(filename)) {
                message("Hashtable loaded successfully!");
            } else {
                message("Error loading hashtable!");
            }
        }
        else if (choice == 8) {
            message("Exiting program...");
        }
        else {
            message("Invalid Option");
        }

    } while (choice != 8);
    return 0;
}
