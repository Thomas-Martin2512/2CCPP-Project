#include "../../include/Display_Board/Display_Board.hpp"
#include "../../include/Game/Game.hpp"
#include <iostream>
#include <iomanip>

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

    std::cout << "+";
    for (int j = 0; j < cols; ++j) std::cout << "--";
    std::cout << "+" << std::endl;

    for (int i = 0; i < rows; ++i) {
        std::cout << "|";

        for (int j = 0; j < cols; ++j) {
            auto it = bonuses.find({i, j});
            if (it != bonuses.end()) {
                std::cout << it->second->getSymbol() << " ";
            }
            else {
                char cell = grid[i][j];

                if (cell == '#') {
                    int owner = ownerGrid[i][j];
                    if (owner > 0 && owner <= static_cast<int>(players.size())) {
                        std::string ansi = game.getAnsiColor(players[owner-1].getColor());
                        std::cout << ansi << "#" << "\033[0m ";
                    } else {
                        std::cout << "# ";
                    }
                }
                else {
                    std::cout << ". ";
                }
            }
        }

        std::cout << "|" << std::endl;
    }

    std::cout << "+";
    for (int j = 0; j < cols; ++j) std::cout << "--";
    std::cout << "+" << std::endl;
}
