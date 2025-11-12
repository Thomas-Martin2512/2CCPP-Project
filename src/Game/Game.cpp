#include "../../include/Game/Game.hpp"
#include "../include/Cursor/CursorController.hpp"
#include "../include/Display_Board/Display_Board.hpp"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>

Game::Game() : board(), display(nullptr), currentRound(1) {}

void Game::start() {
    std::cout << "=== DÉBUT DE LA PARTIE ===" << std::endl;

    setupPlayers();
    setupBoard();
    placeStartingTiles();

    std::cout << "\nTous les joueurs ont placé leur tuile de départ !" << std::endl;
    displayBoard();

    std::cout << "\n--- Début des 9 manches ---" << std::endl;
}

void Game::setupPlayers() {
    int numberOfPlayers;
    do {
        std::cout << "Entrez le nombre de joueurs (2 à 9) : ";
        std::cin >> numberOfPlayers;
        std::cin.ignore();
    } while (numberOfPlayers < 2 || numberOfPlayers > 9);

    std::vector<std::string> availableColors = {
        "rouge", "bleu", "vert", "jaune",
        "cyan", "magenta", "blanc", "gris", "orange"
    };

    for (int i = 0; i < numberOfPlayers; ++i) {
        std::string name;
        std::string color;
        bool validColor = false;

        std::cout << "\n=== Joueur " << (i + 1) << " ===" << std::endl;
        std::cout << "Entrez le nom du joueur : ";
        std::getline(std::cin, name);

        while (!validColor) {
            std::cout << "\nCouleurs disponibles : ";
            for (const auto& c : availableColors) {
                std::cout << c << " ";
            }
            std::cout << "\nChoisissez une couleur parmi celles disponibles : ";
            std::getline(std::cin, color);

            auto it = std::find_if(
                availableColors.begin(),
                availableColors.end(),
                [&](const std::string& c) {
                    std::string lowerColor = color;
                    std::transform(lowerColor.begin(), lowerColor.end(), lowerColor.begin(), ::tolower);
                    return c == lowerColor;
                }
            );

            if (it != availableColors.end()) {
                validColor = true;
                availableColors.erase(it);
            } else {
                std::cout << "Couleur invalide. Veuillez choisir parmi celles proposées." << std::endl;
            }
        }

        Player p(name, color);
        players.push_back(p);
        std::cout << "Joueur " << name << " enregistré avec la couleur " << color << " !" << std::endl;
    }

    shufflePlayerOrder();
}

void Game::setupBoard() {
    int numPlayers = static_cast<int>(players.size());
    board = Board(numPlayers);
    board.placeBonus(numPlayers);
    display = new Display_Board(board);
}


void Game::shufflePlayerOrder() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::random_shuffle(players.begin(), players.end());
    std::cout << "\nOrdre de jeu aléatoire :" << std::endl;
    for (size_t i = 0; i < players.size(); ++i) {
        std::cout << i + 1 << ". " << players[i].getName() << std::endl;
    }
}

void Game::placeStartingTiles() {
    std::cout << "\n=== Placement des tuiles de départ ===" << std::endl;

    CursorController cursor(board, *this);

    for (auto& player : players) {
        std::cout << player.getName() << ", c'est votre tour !" << std::endl;
        cursor.moveAndPlaceTile(player.getID());

        display->display(*this);
    }
}


void Game::displayBoard() const {
    if (display)
        display->display(*this);
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
