//
// Created by thoma on 28/10/2025.
//



TEST(Utils, GreetReturnsExpectedString) {
    EXPECT_EQ(app::greet("Bob"), "Hello, Bob!");
}