#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
static void enableAnsiOnWindows() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h == INVALID_HANDLE_VALUE) return;
    DWORD mode = 0;
    if (!GetConsoleMode(h, &mode)) return;
    SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#else
static void enableAnsiOnWindows() {}
#endif

#include "../../include/Game/Game.hpp"
#include "../../include/Render/Renderer.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <limits>

static std::string colToLetters(int index) {
    std::string result;
    while (index >= 0) {
        char letter = 'A' + (index % 26);
        result = letter + result;
        index = (index / 26) - 1;
    }
    return result;
}

static int lettersToCol(const std::string& in) {
    int val = 0;
    bool any = false;
    for (char ch : in) {
        if (std::isspace((unsigned char)ch)) continue;
        ch = char(std::toupper((unsigned char)ch));
        if (ch < 'A' || ch > 'Z') return -1;
        val = val * 26 + (ch - 'A' + 1);
        any = true;
    }
    if (!any) return -1;
    return val - 1;
}


Game::Game()
    : board(),
      display(nullptr),
      queue(true),
      currentRound(1),
      currentPlayerIndex(0),
      gameOver(false) {}

Game::~Game() {
    delete display;
}

void Game::start() {
    enableAnsiOnWindows();
    std::cout << " THE GAME IS STARTING \n";

    setupPlayers();
    setupBoard();
    setupTiles();
    placeStartingTiles();

    displayBoard();
    runRounds(9);

    std::cout << "\n THE GAME IS OVER \n";
}

/* ---------------------- SETUP ---------------------- */

void Game::setupPlayers() {
    int numberOfPlayers = readIntInRange("How many players will play (2 to 9) ? : ", 2, 9);

    std::vector<std::string> availableColors = {
        "red", "blue", "green", "yellow",
        "cyan", "violet", "white", "gray", "orange"
    };

    players.clear();
    players.reserve(numberOfPlayers);

    for (int i = 0; i < numberOfPlayers; ++i) {
        std::string name;
        std::string color;
        bool validColor = false;

        std::cout << "\n Player : " << (i + 1) << "\n";
        std::cout << "Choose a name : ";
        std::getline(std::cin, name);
        if (name.empty()) { --i; continue; }

        while (!validColor) {
            std::cout << "Available colors : ";
            for (const auto& c : availableColors) std::cout << c << " ";
            std::cout << "\nChoose a color among those available : ";
            std::getline(std::cin, color);
            std::string lc = color;
            std::transform(lc.begin(), lc.end(), lc.begin(), ::tolower);

            auto it = std::find(availableColors.begin(), availableColors.end(), lc);
            if (it != availableColors.end()) {
                validColor = true;
                availableColors.erase(it);
                color = lc;
            } else {
                std::cout << "Invalid color. Try again.\n";
            }
        }

        players.emplace_back(name, color);
        std::cout << "Player : " << name << " saved along with its color : " << color << " !\n";
    }

    std::random_device rd; std::mt19937 g(rd());
    std::shuffle(players.begin(), players.end(), g);
    announceOrder();
}

void Game::announceOrder() const {
    std::cout << "\nPlayer positioning :\n";
    for (size_t i = 0; i < players.size(); ++i) {
        std::cout << i + 1 << ". " << players[i].getName() << "\n";
    }
}

void Game::setupBoard() {
    int numPlayers = static_cast<int>(players.size());
    board = Board(numPlayers);
    board.placeBonus(numPlayers);
    display = new Display_Board(board);
}

void Game::setupTiles() {
    if (!tileSet.loadFromFile("Shapes.json")) {
        std::cerr << "Error : cannot load Shapes.kson\n";
        std::exit(1);
    }
    queue.initFrom(tileSet, /*shuffle=*/true);
    std::cout << "Loaded Tiles : " << tileSet.all().size() << "\n";

    auto ids = queue.nextTileIds(5);
    std::cout << "\n";
}

void Game::placeStartingTiles() {
    std::cout << "\n=== Placing the starting tiles ===\n";

    for (auto& player : players) {
        bool valid = false;
        char colChar;
        int row;

        while (!valid) {
            displayBoard();
            std::cout << player.getName() << " (" << player.getColor()
                      << "), Enter the position of your starting tile (Column Letter / Row Number) : ";
            std::cin >> colChar >> row;

            int col = toupper(colChar) - 'A';

            if (row >= 0 && row < board.getRows() && col >= 0 && col < board.getCols()) {
                if (board.getGrid()[row][col] == '.') {
                    valid = true;

                    auto& grid = const_cast<std::vector<std::vector<char>>&>(board.getGrid());
                    grid[row][col] = '#';

                    auto& owner = const_cast<std::vector<std::vector<int>>&>(board.getOwnerGrid());
                    owner[row][col] = player.getID();

                    std::cout << "Tile placed in " << colChar << row << "\n\n";
                } else {
                    std::cout << "This slot is already taken, please choose another position.\n";
                }
            } else {
                std::cout << "Invalid coordinates, column between A-"
                          << static_cast<char>('A' + board.getCols() - 1)
                          << " and line between 0-" << board.getRows() - 1 << ".\n";
            }
        }
    }

    std::cout << "All the starting tiles have been placed!\n";
    displayBoard();
}

/* ---------------------- BOUCLE DE JEU ---------------------- */

void Game::runRounds(int maxRounds) {
    for (currentRound = 1; currentRound <= maxRounds && !gameOver; ++currentRound) {
        std::cout << "\n Round " << currentRound << " \n";
        for (currentPlayerIndex = 0;
             currentPlayerIndex < static_cast<int>(players.size()) && !gameOver;
             ++currentPlayerIndex) {
            playTurn(players[currentPlayerIndex]);
        }
        endRound();
    }
}

void Game::playTurn(Player& player) {
    std::cout << "\n" << player.getName() << "'s turn \n";

    Tile current = queue.draw();
    showQueueWithCurrent(current);

    // Boucle d’actions sur la tuile avant placement
    // Actions autorisées:
    //   p = placer
    //   e = échanger contre fenêtre [0..4]
    //   r = rotate (90° antihoraire)
    //   f = flip
    //   q = abandon de la pose (rare, mais on le gère)
    while (true) {
        char cmd = readChoice(
            "Actions :  p = place, e = exchange, r = rotate, f = flip, q = cancel : ",
            "perfq"
        );

        if (cmd == 'r') {
            current.rotate();
            showQueueWithCurrent(current);
        } else if (cmd == 'f') {
            current.flip();
            showQueueWithCurrent(current);
        } else if (cmd == 'e') {
            if (promptExchange(current)) {
                showQueueWithCurrent(current);
            }
        } else if (cmd == 'p') {
            if (promptPlace(current, player.getID())) {
                displayBoard();
                break;
            } else {
                std::cout << "Invalid placement or cancelled.\n";
                showQueueWithCurrent(current);
            }
        } else if (cmd == 'q') {
            std::cout << "Cancelled move. Tile lost for this round.\n";
            break;
        }
    }
}

void Game::endRound() {
    if (isGameOver()) {
        gameOver = true;
    }
}

bool Game::isGameOver() const {
    return false;
}

/* ---------------------- ACTIONS TUILE ---------------------- */

void Game::showQueueWithCurrent(const Tile& current) const {
    std::cout << "\n";
    queue.display(std::cout, current, 5);
}

void Game::promptTransform(Tile& current) const {
    // Non utilisée directement (on a les actions r/f), mais dispo si tu veux proposer un menu guidé.
}

bool Game::promptExchange(Tile& current) {
    auto ids = queue.nextTileIds(5);

    int index = readIntInRange("Choose the tile you want to exchange with your current one (1 to 5) : ", 1, 5);

    auto newTile = queue.exchangeWithWindow(static_cast<size_t>(index-1), current.getId());
    if (!newTile) {
        std::cout << "Cannot exchange tile (invalid index).\n";
        return false;
    }
    current = *newTile;
    return true;
}

bool Game::promptPlace(Tile& current, int playerId) {
    int x = -1;
    while (true) {
        char maxColChar = 'A' + std::min(board.getCols(), 26) - 1;
        std::cout << "X origin (A-";
        if (board.getCols() <= 26) std::cout << maxColChar;
        else std::cout << colToLetters(board.getCols() - 1);
        std::cout << ") : ";

        std::string s; std::getline(std::cin, s);
        x = lettersToCol(s);
        if (x >= 0 && x < board.getCols()) break;

        std::cout << "Invalid column. Please enter letters between A and "
                  << colToLetters(board.getCols() - 1) << ".\n";
    }

    int y = readIntInRange("Y origin (0 to rows-1) : ", 0, board.getRows() - 1);

    auto pts = current.footprint(x, y, 0, false);
    if (!canPlaceFootprint(pts, playerId)) {
        std::cout << "Invalid position (collision or out of bounds).\n";
        return false;
    }
    placeFootprint(pts, playerId);
    return true;
}

/* ---------------------- I/O ROBUSTES ---------------------- */

int Game::readIntInRange(const std::string& prompt, int minVal, int maxVal) {
    while (true) {
        std::cout << prompt;
        int v;
        if (std::cin >> v) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (v >= minVal && v <= maxVal) return v;
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cout << "Invalid input. Try again (" << minVal << ".." << maxVal << ").\n";
    }
}

bool Game::readYesNo(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string s;
        std::getline(std::cin, s);
        if (s.size()) {
            char c = static_cast<char>(::tolower(s[0]));
            if (c=='y' || c=='o') return true;
            if (c=='n') return false;
        }
        std::cout << "Entree invalide. Tapez y/oui ou n/non.\n";
    }
}

char Game::readChoice(const std::string& prompt, const std::string& allowed) {
    while (true) {
        std::cout << prompt;
        std::string s;
        std::getline(std::cin, s);
        if (!s.empty()) {
            char c = static_cast<char>(::tolower(s[0]));
            if (allowed.find(c) != std::string::npos) return c;
        }
        std::cout << "Entree invalide. Choix acceptes: ";
        for (char a : allowed) std::cout << a << " ";
        std::cout << "\n";
    }
}

/* ---------------------- AIDES PLACEMENT ---------------------- */

bool Game::canPlaceFootprint(const std::vector<std::pair<int,int>>& pts, int playerId) const {
    const auto& grid      = board.getGrid();
    const auto& ownerGrid = board.getOwnerGrid();
    const int rows = board.getRows();
    const int cols = board.getCols();

    for (auto [x,y] : pts) {
        if (x < 0 || x >= cols || y < 0 || y >= rows) return false;
        if (grid[y][x] != '.') return false;
    }

    static const int DX[4] = {1,-1,0,0};
    static const int DY[4] = {0,0,1,-1};

    bool touchesOwn = false;

    for (auto [x,y] : pts) {
        for (int k = 0; k < 4; ++k) {
            int nx = x + DX[k], ny = y + DY[k];
            if (nx < 0 || nx >= cols || ny < 0 || ny >= rows) continue;
            int owner = ownerGrid[ny][nx];
            if (owner != 0) {
                if (owner != playerId) {
                    return false;
                } else {
                    touchesOwn = true;
                }
            }
        }
    }

    return touchesOwn;
}

void Game::placeFootprint(const std::vector<std::pair<int,int>>& pts, int playerId) {
    for (auto [x,y] : pts) {
        board.placeTile(x, y, playerId);
        board.checkBonusCapture(x, y, playerId);
    }
    std::cout << "Tuile placee (" << pts.size() << " cases).\n";
}

/* ---------------------- AFFICHAGE ---------------------- */

void Game::displayBoard() const {
    if (display) display->display(*this);
}

std::string Game::getAnsiColor(const std::string& colorName) const {
    if (colorName == "rouge"   || colorName == "red")      return "\033[31m";
    if (colorName == "bleu"    || colorName == "blue")     return "\033[34m";
    if (colorName == "vert"    || colorName == "green")    return "\033[32m";
    if (colorName == "jaune"   || colorName == "yellow")   return "\033[33m";
    if (colorName == "cyan")                               return "\033[36m";
    if (colorName == "magenta" || colorName == "violet")   return "\033[35m";
    if (colorName == "blanc"   || colorName == "white")    return "\033[37m";
    if (colorName == "gris"    || colorName == "gray")     return "\033[90m";
    if (colorName == "orange")                             return "\033[38;5;208m";
    if (colorName == "rose"    || colorName == "pink")     return "\033[38;5;213m";
    return "\033[0m";
}