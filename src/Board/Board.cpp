#include "../../include/Board/Board.hpp"
#include "string"
#include "iostream"
#include "../../include/Bonus/ExchangeBonus.hpp"
#include "../../include/Bonus/StoneBonus.hpp"
#include "../../include/Bonus/SteelBonus.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "../../include/Player/Player.hpp"
#include "../../include/Bonus/Bonus.hpp"
#include "../../include/Game/Game.hpp"

/**
 * @brief Constructeur par défaut.
 *
 * Initialise un plateau vide avec 0 ligne et 0 colonne.
 * La grille devra être initialisée ensuite via initGrid().
 */
Board::Board() : rows(0), cols(0) {}

/**
 * @brief Constructeur qui initialise directement la taille du plateau.
 *
 * Appelle initGrid(numberOfPlayers) pour configurer la grille
 * selon le nombre de joueurs.
 *
 * @param numberOfPlayers Nombre de joueurs dans la partie (2 à 9).
 */
Board::Board(int numberOfPlayers) {
    initGrid(numberOfPlayers);
}

/**
 * @brief Initialise la taille et le contenu de la grille.
 *
 * - De 2 à 4 joueurs : plateau 20x20.
 * - De 5 à 9 joueurs : plateau 30x30.
 * - Sinon : plateau invalide (0x0) et message d’erreur.
 *
 * Les cases sont initialisées à :
 * - '.' pour la grille d’affichage ;
 * - 0 dans ownerGrid (aucun propriétaire).
 *
 * @param numberOfPlayers Nombre de joueurs.
 */
void Board::initGrid(int numberOfPlayers) {
    if (numberOfPlayers >= 2 && numberOfPlayers <= 4) {
        rows = 20;
        cols = 20;
    } else if (numberOfPlayers >= 5 && numberOfPlayers <= 9) {
        rows = 30;
        cols = 30;
    } else {
        std::cerr << "Nombre de joueurs invalide !" << std::endl;
        rows = cols = 0;
    }

    grid = std::vector<std::vector<char>>(rows, std::vector<char>(cols, '.'));
    ownerGrid = std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0));
}

/**
 * @brief Place aléatoirement les cases bonus sur le plateau.
 *
 * Le nombre de bonus dépend du nombre de joueurs :
 * - Cases échange ('E') : 1,5 par joueur (arrondi supérieur) ;
 * - Cases pierre  ('R') : 0,5 par joueur (arrondi supérieur) ;
 * - Cases vol     ('S') : 1 par joueur.
 *
 * Règles de placement :
 * - Les bonus ne peuvent pas être sur le bord ;
 * - Deux bonus ne peuvent pas être adjacents (zone 3x3 interdite).
 *
 * @param numberOfPlayers Nombre de joueurs.
 */
void Board::placeBonus(int numberOfPlayers) {
    if (rows == 0 || cols == 0) return;

    srand(static_cast<unsigned>(time(nullptr)));

    int nbExchange = std::ceil(1.5 * numberOfPlayers);
    int nbStone = std::ceil(0.5 * numberOfPlayers);
    int nbSteal = numberOfPlayers;

    auto place = [&](std::shared_ptr<Bonus> bonus) {
        int x, y;
        bool valid = false;
        while (!valid) {
            x = rand() % (rows - 2) + 1;
            y = rand() % (cols - 2) + 1;
            valid = (grid[x][y] == '.') && (bonuses.find({x,y}) == bonuses.end());
            if (valid) {
                for (int dx = -1; dx <= 1; ++dx)
                    for (int dy = -1; dy <= 1; ++dy)
                        if (bonuses.find({x+dx, y+dy}) != bonuses.end())
                            valid = false;
            }
        }
        bonuses[{x,y}] = bonus;
    };

    for (int i = 0; i < nbExchange; ++i) place(std::make_shared<ExchangeBonus>());
    for (int i = 0; i < nbStone; ++i)   place(std::make_shared<StoneBonus>());
    for (int i = 0; i < nbSteal; ++i)   place(std::make_shared<StealBonus>());
}

/**
 * @brief Accès en lecture aux bonus placés sur la grille.
 *
 * @return Référence constante vers la map des bonus :
 *         clé = position (x,y), valeur = pointeur sur Bonus.
 */
const std::map<std::pair<int,int>, std::shared_ptr<Bonus>>& Board::getBonus() const {
    return bonuses;
}


/**
 * @brief Place une pierre (obstacle) 1x1 sur le plateau.
 *
 * La pierre est représentée par le caractère 'X' et n’appartient à aucun joueur
 * (ownerGrid = 0). Elle ne peut être placée que sur une case vide ('.').
 *
 * @param x Coordonnée de colonne.
 * @param y Coordonnée de ligne.
 * @return true si la pierre a été placée, false sinon (hors limites ou case occupée).
 */
void Board::placeTile(int x, int y, int playerId) {
    if (x >= 0 && x < cols && y >= 0 && y < rows && grid[y][x] == '.') {
        grid[y][x] = '#';
        ownerGrid[y][x] = playerId;
    }
}

/**
 * @brief Place une pierre (obstacle) 1x1 sur le plateau.
 *
 * La pierre est représentée par le caractère 'X' et n’appartient à aucun joueur
 * (ownerGrid = 0). Elle ne peut être placée que sur une case vide ('.').
 *
 * @param x Coordonnée de colonne.
 * @param y Coordonnée de ligne.
 * @return true si la pierre a été placée, false sinon (hors limites ou case occupée).
 */
bool Board::placeStone(int x, int y) {
    if (x < 0 || x >= cols || y < 0 || y >= rows) return false;
    if (grid[y][x] != '.') return false;

    grid[y][x] = 'X';
    ownerGrid[y][x] = 0;
    return true;
}

/**
 * @brief Convertit une lettre de colonne (A, B, C, …) en index de colonne.
 *
 * @param letter Chaîne contenant au moins un caractère alphabétique.
 * @return Index de colonne (0 pour 'A', 1 pour 'B', …), ou -1 si invalide.
 */
int Board::letterToCol(const std::string& letter) {
    if (letter.empty()) return -1;
    char c = toupper(letter[0]);
    if (c < 'A' || c > 'Z') return -1;
    return c - 'A';
}

/**
 * @brief Vérifie si un ou plusieurs bonus ont été capturés par un joueur.
 *
 * Un bonus est considéré comme capturé si les quatre cases cardinales
 * (haut, bas, gauche, droite) autour de sa position appartiennent toutes
 * au même joueur.
 *
 * Effets :
 * - La case bonus devient une case de territoire du joueur ('#') ;
 * - Bonus 'E' : le joueur gagne un ticket d’échange ;
 * - Bonus 'R' : le joueur gagne un "Rock bonus" (possibilité de placer une pierre) ;
 * - Bonus 'S' : le joueur gagne un "Stealth bonus" (vol de tuile, à implémenter).
 *
 * Les bonus capturés sont retirés de la map `bonuses`.
 *
 * @param playerId Identifiant du joueur qui vient de jouer.
 * @param game Référence au jeu (utilisé pour accéder au Player).
 */
void Board::checkBonusCapture(int playerId, Game& game) {
    static const std::vector<std::pair<int,int>> directions = {
        {1,0}, {-1,0}, {0,1}, {0,-1}
    };

    std::vector<std::pair<int,int>> capturedKeys;

    for (auto& [pos, bonusPtr] : bonuses) {
        int bx = pos.first;
        int by = pos.second;

        bool surrounded = true;
        for (auto [dx, dy] : directions) {
            int nx = bx + dx;
            int ny = by + dy;

            if (nx < 0 || ny < 0 || nx >= cols || ny >= rows || ownerGrid[ny][nx] != playerId) {
                surrounded = false;
                break;
            }
        }

        if (surrounded) {
            grid[by][bx]      = '#';
            ownerGrid[by][bx] = playerId;

            std::cout << "Bonus captured by player " << playerId
                      << " : " << bonusPtr->getName() << std::endl;

            Player& player = game.getPlayerById(playerId);

            if (bonusPtr->getSymbol() == "E") {
                player.addExchangeCoupon();
                std::cout << "Exchange-ticket +1. Total : "
                          << player.getExchangeCoupons() << "\n";
            }

            if (bonusPtr->getSymbol() == "R") {
                player.setRockBonusAvailable(true);
                std::cout << "Rock bonus : available (you can place a 1x1 stone on an empty cell).\n";
            }

            if (bonusPtr->getSymbol() == "S") {
                player.setStealthBonusAvailable(true);
                std::cout << "Stealth bonus : available.\n";
            }

            capturedKeys.push_back(pos);
        }
    }

    for (auto& key : capturedKeys) {
        bonuses.erase(key);
    }
}

/**
 * @brief Affiche la grille brute dans la sortie standard.
 *
 * Cette méthode n’utilise pas de couleurs ni d’informations de propriétaire,
 * elle est surtout utile pour le débogage.
 */
void Board::displayGrid() const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << grid[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Retourne le nombre de lignes du plateau.
 * @return Nombre de lignes.
 */
int Board::getRows() const { return rows; }

/**
 * @brief Retourne le nombre de colonnes du plateau.
 * @return Nombre de colonnes.
 */
int Board::getCols() const { return cols; }

/**
 * @brief Accès en lecture à la grille de caractères.
 *
 * @return Référence constante vers la matrice de caractères ('.', '#', 'X', etc.).
 */
const std::vector<std::vector<char>>& Board::getGrid() const { return grid; }

/**
 * @brief Vérifie si un ensemble de cases est plaçable sur le plateau (grille uniquement).
 *
 * Teste uniquement :
 * - les limites du plateau ;
 * - le fait que toutes les cases correspondantes soient vides ('.').
 *
 * @param pts Liste des coordonnées (x,y) à tester.
 * @param playerId Identifiant du joueur (actuellement inutilisé).
 * @return true si toutes les cases sont dans la grille et libres, false sinon.
 */
bool Board::canPlaceFootprint(const std::vector<std::pair<int,int>>& pts, int /*playerId*/) const {
    for (auto [x,y] : pts) {
        if (x < 0 || x >= cols || y < 0 || y >= rows) return false;
        if (grid[y][x] != '.') return false;
    }
    return true;
}

/**
 * @brief Pose effectivement une empreinte (ensemble de cases) sur le plateau.
 *
 * Si canPlaceFootprint(...) échoue, aucune modification n’est effectuée.
 * En cas de succès, chaque case est marquée par '#'
 * et son propriétaire est mis à playerId dans ownerGrid.
 *
 * @param pts Coordonnées absolues (x,y) des cases à occuper.
 * @param playerId Identifiant du joueur propriétaire.
 * @return true si le placement a réussi, false sinon.
 */
bool Board::placeFootprint(const std::vector<std::pair<int,int>>& pts, int playerId) {
    if (!canPlaceFootprint(pts, playerId)) return false;
    for (auto [x,y] : pts) {
        grid[y][x] = '#';
        ownerGrid[y][x] = playerId;
    }
    return true;
}