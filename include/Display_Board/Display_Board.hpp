#ifndef DISPLAY_BOARD_HPP_INCLUDED
#define DISPLAY_BOARD_HPP_INCLUDED
#include "../Board/Board.hpp"
#include <iostream>
#include "Game/Game.hpp"


/**
 * @class Display_Board
 * @brief Gère l’affichage graphique ASCII du plateau de jeu.
 *
 * Cette classe se charge d’afficher :
 *  - la grille du plateau ;
 *  - les indices de colonnes (A, B, …, Z, AA…) ;
 *  - les indices de lignes ;
 *  - les zones occupées par les joueurs, colorées via ANSI ;
 *  - les bonus présents sur le plateau ;
 *  - les bordures supérieures et inférieures.
 *
 * Elle ne modifie en aucun cas l’état du jeu :
 * elle n’est utilisée que pour l’affichage.
 */
class Game;

class Display_Board {
    public :

        /**
        * @brief Construit un objet d’affichage basé sur un plateau.
        * @param board Référence constante vers le plateau à afficher.
        *
        * Le plateau est conservé tel quel et ne peut pas être modifié
        * via Display_Board.
        */
        explicit Display_Board(const Board& board);

        /**
        * @brief Affiche la grille complète du plateau, colorée selon les joueurs.
        *
        * @param game Référence vers l’objet Game afin d’obtenir :
        *     - informations joueurs (couleurs ANSI),
        *     - propriétaires de cases,
        *     - bonus capturés ou non.
        */
        void display(const Game& game) const;

    private:
        const Board& board;

        /**
         * @brief Affiche la bordure supérieure du plateau (ligne +---…+).
         * @param cols Nombre de colonnes du plateau.
         */
        void printTopBoarder(int cols) const;

        /**
         * @brief Affiche la bordure inférieure du plateau.
         *
         * Identique à la bordure supérieure, mais séparée pour plus
         * de clarté et future extensibilité.
         *
         * @param cols Nombre de colonnes du plateau.
         */
        void printBottomBoarder(int cols) const;
};


#endif // DISPLAY_BOARD_HPP_INCLUDED