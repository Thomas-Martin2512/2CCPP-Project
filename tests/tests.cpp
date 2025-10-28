#include <gtest/gtest.h>
#include "Utils_example.hpp"

TEST(Utils, GreetReturnsExpectedString) {
    EXPECT_EQ(app::greet("Bob"), "Hello, Bob!");
}