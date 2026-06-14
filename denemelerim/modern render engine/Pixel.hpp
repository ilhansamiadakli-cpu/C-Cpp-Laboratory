#pragma once
#include <string>

struct Pixel {
    char character = ' ';
    std::string color = "\033[0m"; // Varsayılan RESET rengi
};