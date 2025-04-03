#pragma once

#include <string>

struct Step {
    int highlightedIndex;    // Ô highlight
    int codeLine;            // Dòng code được highlight
    std::string description; // Mô tả bước
};