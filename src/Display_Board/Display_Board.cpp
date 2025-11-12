#include "../../include/Display_Board/Display_Board.hpp"
#include "../../include/Game/Game.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cctype>

static std::string colToLetters(int index) {
    std::string result;
    while (index >= 0) {
        char letter = 'A' + (index % 26);
        result = letter + result;
        index = (index / 26) - 1;
    }
    return result;
}

Display_Board::Display_Board(const Board &board) : board(board) {}

void Display_Board::printTopBoarder(int cols) const {
    std::cout << "  +";
    for (int j = 0; j < cols; ++j)
        std::cout << "--";
    std::cout << "+" << std::endl;
}


void Display_Board::printBottomBoarder(int cols) const {
    printTopBoarder(cols);
}

void Display_Board::display(const Game& game) const {
    const auto& grid = board.getGrid();
    const auto& ownerGrid = board.getOwnerGrid();
    const auto& bonuses = board.getBonus();
    const auto& players = game.getPlayers();

    int rows = board.getRows();
    int cols = board.getCols();

    if (rows == 0 || cols == 0) {
        std::cerr << "Error: board is empty." << std::endl;
        return;
    }

    std::cout << "   ";
    for (int j = 0; j < cols; ++j) {
        std::string lab = colToLetters(j);
        std::cout << std::setw(2) << lab;
    }
    std::cout << "\n";

    std::cout << "   +";
    for (int j = 0; j < cols; ++j) std::cout << "--";
    std::cout << "+\n";

    for (int i = 0; i < rows; ++i) {
        std::cout << std::setw(2) << i << " |";
        for (int j = 0; j < cols; ++j) {
            char cell = grid[i][j];
            int owner = ownerGrid[i][j];

            auto it = bonuses.find({j, i});
            if (it != bonuses.end()) {
                if (owner > 0 && owner <= static_cast<int>(players.size())) {
                    std::string ansi = game.getAnsiColor(players[owner - 1].getColor());
                    std::cout << ansi << it->second->getSymbol() << "\033[0m ";
                } else {
                    std::cout << it->second->getSymbol() << " ";
                }
            } else if (cell == '#') {
                if (owner > 0 && owner <= static_cast<int>(players.size())) {
                    std::string ansi = game.getAnsiColor(players[owner - 1].getColor());
                    std::cout << ansi << "#" << "\033[0m ";
                } else {
                    std::cout << "# ";
                }
            } else {
                std::cout << ". ";
            }
        }
        std::cout << "|\n";
    }

    std::cout << "   +";
    for (int j = 0; j < cols; ++j) std::cout << "--";
    std::cout << "+\n";
}

