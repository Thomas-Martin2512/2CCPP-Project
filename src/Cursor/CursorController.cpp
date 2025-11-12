#include "../include/Cursor/CursorController.hpp"
#include "../include/Game/Game.hpp"
#include "../include/Board/Board.hpp"
#include "../include/Display_Board/Display_Board.hpp"
#ifdef _WIN32
#include <conio.h>
#define CLEAR_SCREEN "cls"
#else
#include <termios.h>
#include <unistd.h>
#define CLEAR_SCREEN "clear"
#endif
class Game;


CursorController::CursorController(Board& b, Game& g)
    : board(b), game(g), cursorX(0), cursorY(0) { }

int CursorController::getArrowKey() const {
#ifdef _WIN32
    int ch = _getch();
    if (ch == 0 || ch == 224) {
        switch (_getch()) {
            case 72: return 0;
            case 80: return 1;
            case 75: return 2;
            case 77: return 3;
        }
    } else if (ch == 13) return 4;
    return -1;
#else
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    switch (ch) {
        case 'w': return 0;
        case 's': return 1;
        case 'a': return 2;
        case 'd': return 3;
        case '\n': return 4;
        default: return -1;
    }
#endif
}

void CursorController::drawBoard(int playerId) const {
    int ret = system(CLEAR_SCREEN);
    (void)ret;

    const auto& grid = board.getGrid();
    const auto& ownerGrid = board.getOwnerGrid();
    const auto& players = game.getPlayers();

    for (int i = 0; i < board.getRows(); ++i) {
        for (int j = 0; j < board.getCols(); ++j) {
            if (i == cursorY && j == cursorX) std::cout << "[";
            else std::cout << " ";

            char cell = grid[i][j];
            if (cell == '#') {
                int owner = ownerGrid[i][j];
                if (owner > 0 && owner <= static_cast<int>(players.size())) {
                    std::string ansi = game.getAnsiColor(players[owner - 1].getColor());
                    std::cout << ansi << "#" << "\033[0m";
                } else {
                    std::cout << "#";
                }
            } else {
                std::cout << cell;
            }

            if (i == cursorY && j == cursorX) std::cout << "]";
            else std::cout << " ";
        }
        std::cout << std::endl;
    }
}

void CursorController::moveAndPlaceTile(int playerId) {
    int key;
    do {
        drawBoard(playerId);
        key = getArrowKey();
        switch (key) {
            case 0: if (cursorY > 0) cursorY--; break;            // UP
            case 1: if (cursorY < board.getRows()-1) cursorY++; break; // DOWN
            case 2: if (cursorX > 0) cursorX--; break;            // LEFT
            case 3: if (cursorX < board.getCols()-1) cursorX++; break; // RIGHT
            case 4: // Enter
                if (board.getGrid()[cursorY][cursorX] == '.') {
                    board.placeTile(cursorX, cursorY, playerId);
                    return;
                }
                break;
        }
    } while (true);
}
