#include "functions.h"

void message(const std::string& text)
    {
        int length = text.length();

        // top lines
        std::cout << "+";
        for (int i = 0; i < length + 2; i++) std::cout << "-";
        std::cout << "+\n";

        // left and right lines
        std::cout << "| " << text << " |\n";

        // bottom lines
        std::cout << "+";
        for (int i = 0; i < length + 2; i++) std::cout << "-";
        std::cout << "+\n\n";

        delay(2000);
    }

void delay(int milliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
