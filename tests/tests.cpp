//
// Created by thoma on 28/10/2025.
//

// Native library import
#include <iostream>

// Folders / Files import
#include "Utils_example.hpp"

TEST(Utils, GreetReturnsExpectedString) {
    EXPECT_EQ(app::greet("Bob"), "Hello, Bob!");
}