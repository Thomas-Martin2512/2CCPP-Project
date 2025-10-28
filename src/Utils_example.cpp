// Utils_example.cpp

// Native library import
#include <iostream>

// Folders / Files import
#include "Utils_example.hpp"

namespace app {
    std::string greet(const std::string& name) {
        return "Hello, " + name + "!";
    }
}