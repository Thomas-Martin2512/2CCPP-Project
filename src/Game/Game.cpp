#include "../../include/Game/Game.hpp"
#include "../../include/Render/Renderer.hpp"
#include <iostream>
#include <algorithm>
#include <limits>

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
    std::cout << " THE GAME IS STARTING \n";

    setupPlayers();
    setupBoard();
    setupTiles();

    displayBoard();
    runRounds(9);

    std::cout << "\n THE GAME IS OVER \n";
}

/* ---------------------- SETUP ---------------------- */

void Game::setupPlayers() {
    int numberOfPlayers = readIntInRange("How many players will play (1 to 9) ? : ", 2, 9);

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

    // Affiche les 5 premières IDs pour debug/repère
    auto ids = queue.nextTileIds(5);
    std::cout << "Next 5 tiles-id: ";
    for (auto& id : ids) std::cout << id << " ";
    std::cout << "\n";
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
            "Actions [p=place, e=exchange, r=rotate, f=flip, q=cancel] : ",
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
    std::cout << "Fenetre [0..4] IDs: ";
    for (auto& id : ids) std::cout << id << " ";
    std::cout << "\n";

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
    bool doFlip = readYesNo("Flip the tile before placing it ? (y/n) : ");
    int rot = readIntInRange("Number of 90° rotations (0 to 3) : ", 0, 3);

    int x = readIntInRange("X's origin (0 to cols-1) : ", 0, board.getCols()-1);
    int y = readIntInRange("Y's origin (0 to rows-1) : ", 0, board.getRows()-1);

    auto pts = current.footprint(x, y, rot, doFlip);

    if (!canPlaceFootprint(pts)) {
        std::cout << "Move forbidden (Colliding tile or out of the board).\n";
        return false;
    }

    // Place
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

bool Game::canPlaceFootprint(const std::vector<std::pair<int,int>>& pts) const {
    const auto& grid = board.getGrid();
    int rows = board.getRows();
    int cols = board.getCols();
    for (auto [x,y] : pts) {
        if (x < 0 || x >= cols || y < 0 || y >= rows) return false;
        if (grid[y][x] != '.') return false;
    }
    return true;
}

void Game::placeFootprint(const std::vector<std::pair<int,int>>& pts, int playerId) {
    for (auto [x,y] : pts) {
        board.placeTile(x, y, playerId); // utilise ta méthode existante (une case à la fois)
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