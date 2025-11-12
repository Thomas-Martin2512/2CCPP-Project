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

/* ---------------------- HELPERS ---------------------- */

static std::string trim_ws(const std::string& s) {
    size_t b = 0, e = s.size();
    while (b < e && std::isspace(static_cast<unsigned char>(s[b]))) ++b;
    while (e > b && std::isspace(static_cast<unsigned char>(s[e-1]))) --e;
    return s.substr(b, e - b);
}

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
    finishAndScore();

    std::cout << "\n THE GAME IS OVER \n";
}

/* ---------------------- SETUP ---------------------- */

void Game::setupPlayers() {
    int numberOfPlayers = readIntInRangeStrict("How many players will play (2 to 9) ? : ", 2, 9);

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
    for (auto& player : players) {
        bool valid = false;
        while (!valid) {
            displayBoard();
            std::cout << player.getName() << " (" << player.getColor()
                      << "), Enter your starting tile (Column Letter + Row Number like 'A0'): ";
            std::string input;
            std::getline(std::cin, input);
            if (input.empty()) {
                std::cout << "Empty input. Try again.\n";
                continue;
            }

            std::string letters, digits;
            for (char c : input) {
                if (std::isalpha(static_cast<unsigned char>(c))) letters += c;
                else if (std::isdigit(static_cast<unsigned char>(c))) digits += c;
            }

            if (letters.empty() || digits.empty()) {
                std::cout << "Invalid format. Example: A0 or B12.\n";
                continue;
            }

            int col = lettersToCol(letters);
            int row = std::stoi(digits);

            if (row < 0 || row >= board.getRows() || col < 0 || col >= board.getCols()) {
                std::cout << "Invalid coordinates. Try again.\n";
                continue;
            }

            if (board.getGrid()[row][col] != '.') {
                std::cout << "This slot is already taken.\n";
                continue;
            }

            auto& grid = const_cast<std::vector<std::vector<char>>&>(board.getGrid());
            grid[row][col] = '#';

            auto& owner = const_cast<std::vector<std::vector<int>>&>(board.getOwnerGrid());
            owner[row][col] = player.getID();

            std::cout << "Tile placed in " << letters << row << "\n\n";
            valid = true;
        }
    }

    std::cout << "All starting tiles have been placed ! The Game may begin.\n";
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
        std::string input;
        std::getline(std::cin, input);

        if (!input.empty() && std::all_of(input.begin(), input.end(), ::isdigit)) {
            int v = std::stoi(input);
            if (v >= minVal && v <= maxVal)
                return v;
        }

        std::cout << "Invalid input. Please enter an integer between "
                  << minVal << " and " << maxVal << ".\n";
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
        std::cout << "Wrong input. Type y/n.\n";
    }
}

char Game::readChoice(const std::string& prompt, const std::string& allowed) {
    while (true) {
        std::cout << prompt;
        std::string s;
        std::getline(std::cin, s);

        if (!s.empty()) {
            char c = static_cast<char>(std::tolower(s[0]));
            if (allowed.find(c) != std::string::npos)
                return c;
        }

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::cout << "Invalid choice. Allowed: ";
        for (char a : allowed) std::cout << a << " ";
        std::cout << "\n";
    }
}

std::string Game::trim(const std::string& s) {
    return trim_ws(s);
}

bool Game::parseIntStrict(const std::string& s, int& out) {
    std::string t = trim(s);
    if (t.empty()) return false;
    size_t i = 0;
    if (t[0] == '+' || t[0] == '-') {
        if (t.size() == 1) return false;
        i = 1;
    }
    for (; i < t.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(t[i]))) return false;
    }
    try {
        long long v = std::stoll(t);
        if (v < std::numeric_limits<int>::min() || v > std::numeric_limits<int>::max()) return false;
        out = static_cast<int>(v);
        return true;
    } catch (...) {
        return false;
    }
}

int Game::readIntInRangeStrict(const std::string& prompt, int minVal, int maxVal) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) {
            std::cin.clear();
            continue;
        }
        int v;
        if (parseIntStrict(line, v) && v >= minVal && v <= maxVal) {
            return v;
        }
        std::cout << "Invalid input. Enter an integer between "
                  << minVal << " and " << maxVal << ".\n";
    }
}

bool Game::readColRow(const std::string& prompt, int maxCol, int maxRow, int& outCol, int& outRow) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) {
            std::cin.clear();
            continue;
        }
        line = trim(line);
        if (line.empty()) {
            std::cout << "Empty input.\n";
            continue;
        }

        std::string letters, digits;
        for (char ch : line) {
            if (std::isalpha(static_cast<unsigned char>(ch))) letters.push_back(ch);
            else if (std::isdigit(static_cast<unsigned char>(ch))) digits.push_back(ch);
            else if (std::isspace(static_cast<unsigned char>(ch))) continue;
            else { letters.clear(); digits.clear(); break; }
        }
        if (letters.empty() || digits.empty()) {
            std::cout << "Format must be <LETTER><NUMBER>, 'A0'.\n";
            continue;
        }

        int col = lettersToCol(letters);
        int row;
        if (!parseIntStrict(digits, row)) {
            std::cout << "Row must be an integer.\n";
            continue;
        }

        if (col < 0 || col >= maxCol) {
            std::cout << "Column out of range. Valid: A.." << colToLetters(maxCol-1) << ".\n";
            continue;
        }
        if (row < 0 || row >= maxRow) {
            std::cout << "Row out of range. Valid: 0.." << (maxRow-1) << ".\n";
            continue;
        }

        outCol = col;
        outRow = row;
        return true;
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
    }
}

/* ---------------------- FIN DE PARTIE ---------------------- */

void Game::finishAndScore() {
    auto scores = computeScores();
    printScores(scores);
}

std::vector<Game::FinalScore> Game::computeScores() const {
    const auto& ownerGrid = board.getOwnerGrid();
    int rows = board.getRows();
    int cols = board.getCols();

    std::vector<FinalScore> results;
    results.reserve(players.size());

    for (const auto& p : players) {
        int id = p.getID();
        int totalCells = 0;
        int maxSquare = 0;

        std::vector<std::vector<int>> dp(rows, std::vector<int>(cols, 0));

        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                if (ownerGrid[y][x] == id) {
                    totalCells++;
                    if (x > 0 && y > 0)
                        dp[y][x] = 1 + std::min({ dp[y-1][x], dp[y][x-1], dp[y-1][x-1] });
                    else
                        dp[y][x] = 1;
                    maxSquare = std::max(maxSquare, dp[y][x]);
                }
            }
        }

        results.push_back({ id, maxSquare, totalCells });
    }

    return results;
}

void Game::printScores(const std::vector<FinalScore>& scores) const {
    std::cout << "\n FINAL RESULTS \n";

    auto sorted = scores;
    std::sort(sorted.begin(), sorted.end(),
              [](const FinalScore& a, const FinalScore& b) {
                  if (a.maxSquare != b.maxSquare)
                      return a.maxSquare > b.maxSquare;
                  return a.cellCount > b.cellCount;
              });

    for (const auto& s : sorted) {
        const auto& player = players[s.playerId - 1];
        std::cout << player.getName() << " (" << player.getColor() << ")"
                  << " Biggest square : " << s.maxSquare
                  << ", Total cells : " << s.cellCount << "\n";
    }

    const auto& winner = players[sorted.front().playerId - 1];
    std::cout << "\n The Winner is : " << winner.getName()
              << " (" << winner.getColor() << ")!\n";
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