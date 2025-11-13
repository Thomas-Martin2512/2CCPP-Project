#ifndef BOARD_HPP_INCLUDED
#define BOARD_HPP_INCLUDED
#include <vector>
#include <map>
#include <memory>
#include "Bonus/Bonus.hpp"

class Game;

/**
 * @class Board
 * @brief Représente le plateau de jeu et gère les cases occupées, les propriétaires
 *        et les cases bonus.
 *
 * Le plateau contient :
 * - une grille de caractères (`grid`) pour l’affichage ('.', '#', 'X', etc.) ;
 * - une grille d’identifiants de joueurs (`ownerGrid`) indiquant qui possède chaque case ;
 * - une collection de cases bonus (`bonuses`) placées à des coordonnées précises.
 *
 * La classe fournit des méthodes pour :
 * - initialiser la taille du plateau en fonction du nombre de joueurs ;
 * - placer les bonus aléatoirement ;
 * - poser des tuiles et des pierres ;
 * - vérifier la capture de bonus ;
 * - tester et poser l’empreinte complète d’une tuile.
 */
class Board {

    private:
        int rows;
        int cols;
        /// Grille d’affichage : '.', '#', 'X', etc.
        std::vector<std::vector<char>> grid;
        /// Cases bonus : clé = position (x,y), valeur = pointeur vers un Bonus.
        std::map<std::pair<int,int>, std::shared_ptr<Bonus>> bonuses;
        /// Grille des propriétaires : 0 = aucun, sinon ID du joueur.
        std::vector<std::vector<int>> ownerGrid;

    public:
        /**
        * @brief Constructeur par défaut.
        *
        * Initialise un plateau vide (0x0). Il doit ensuite être configuré
        * via initGrid(numberOfPlayers).
        */
        Board();

        /**
        * @brief Constructeur avec initialisation de la grille.
        *
        * Appelle initGrid(numberOfPlayers) pour fixer les dimensions
        * et allouer les grilles internes.
        *
        * @param numberOfPlayers Nombre de joueurs (2 à 9).
        */
        Board(int numberOfPlayers);

        /**
         * @brief Initialise la taille du plateau en fonction du nombre de joueurs.
         *
         * - 2–4 joueurs : plateau 20x20 ;
         * - 5–9 joueurs : plateau 30x30 ;
         * - sinon : plateau invalide (0x0) et message d’erreur.
         *
         * Les grilles `grid` et `ownerGrid` sont réallouées et remises à zéro.
         *
         * @param numberOfPlayers Nombre de joueurs.
         */
        void initGrid(int numberOfPlayers);

        /**
         * @brief Place aléatoirement les bonus sur le plateau.
         *
         * Le nombre de bonus dépend du nombre de joueurs :
         * - Bonus échange ('E')  : 1,5 par joueur (arrondi supérieur) ;
         * - Bonus pierre  ('R')  : 0,5 par joueur (arrondi supérieur) ;
         * - Bonus vol     ('S')  : 1 par joueur.
         *
         * Les bonus ne sont ni sur les bords du plateau, ni adjacents entre eux.
         *
         * @param numberOfPlayers Nombre de joueurs.
         */
        void placeBonus(int numberOfPlayers);

        /**
         * @brief Affiche la grille brute sur la sortie standard (pour débogage).
         *
         * Cette méthode n’utilise pas de couleurs ni d’informations de joueurs.
         */
        void displayGrid() const;


        /**
         * @brief Pose une case de territoire ('#') pour un joueur.
         *
         * La case doit être vide ('.') et à l’intérieur du plateau.
         *
         * @param x Coordonnée de colonne.
         * @param y Coordonnée de ligne.
         * @param playerId Identifiant du joueur propriétaire.
         */
        void placeTile(int x, int y, int playerId);

        /**
         * @brief Pose une pierre ('X') 1x1 sur le plateau.
         *
         * La case doit être vide. La pierre n’appartient à aucun joueur
         * (ownerGrid = 0) et bloque les placements futurs.
         *
         * @param x Coordonnée de colonne.
         * @param y Coordonnée de ligne.
         * @return true si la pierre a été placée, false sinon.
         */
        bool placeStone(int x, int y);

        /**
         * @brief Vérifie si des bonus sont capturés par un joueur.
         *
         * Un bonus est capturé si les quatre cases cardinales autour de celui-ci
         * appartiennent au joueur. L’effet du bonus est alors appliqué via `game`
         * (tickets d’échange, Rock bonus, Stealth bonus), et la case de bonus
         * devient une case de territoire du joueur.
         *
         * @param playerId Identifiant du joueur qui vient de jouer.
         * @param game Référence vers l’objet Game pour accéder au joueur.
         */
        void checkBonusCapture(int playerId, Game& game);

        /**
         * @brief Retourne le nombre de lignes du plateau.
         * @return Nombre de lignes.
         */
        int getRows() const;

        /**
         * @brief Retourne le nombre de colonnes du plateau.
         * @return Nombre de colonnes.
         */
        int getCols() const;

        /**
         * @brief Accède à la grille de caractères.
         *
         * @return Référence constante vers la matrice `grid`.
         */
        const std::vector<std::vector<char>>& getGrid() const;

        /**
         * @brief Convertit une lettre de colonne en indice (0-based).
         *
         * @param letter Lettre représentant une colonne (ex. "A", "B", ...).
         * @return Index de colonne (0 pour 'A'), ou -1 si invalide.
         */
        static int letterToCol(const std::string& letter);

        /**
         * @brief Accède à la grille des propriétaires.
         *
         * @return Référence constante vers la matrice `ownerGrid`.
         */
        const std::vector<std::vector<int>>& getOwnerGrid() const { return ownerGrid; };

        /**
         * @brief Accède aux bonus placés sur le plateau.
         *
         * @return Référence constante vers la map des bonus.
         */
        const std::map<std::pair<int,int>, std::shared_ptr<Bonus>>& getBonus() const;

        /**
         * @brief Vérifie si une empreinte de tuile est plaçable sur la grille.
         *
         * Contrôle uniquement :
         * - que toutes les coordonnées sont dans les limites ;
         * - que les cases correspondantes sont vides ('.').
         *
         * @param pts Liste de coordonnées (x,y) à tester.
         * @param playerId Identifiant du joueur (actuellement ignoré).
         * @return true si toutes les cases sont libres et dans la grille, false sinon.
         */
        bool canPlaceFootprint(const std::vector<std::pair<int,int>>& pts, int playerId) const;

        /**
         * @brief Pose effectivement l’empreinte d’une tuile sur le plateau.
         *
         * Si le placement est valide (via canPlaceFootprint), chaque case est
         * marquée par '#' et assignée au joueur donné dans `ownerGrid`.
         *
         * @param pts Coordonnées absolues des cases à occuper.
         * @param playerId Identifiant du joueur propriétaire.
         * @return true si le placement a réussi, false sinon.
         */
        bool placeFootprint(const std::vector<std::pair<int,int>>& pts, int playerId);
};


#endif // BOARD_HPP_INCLUDED