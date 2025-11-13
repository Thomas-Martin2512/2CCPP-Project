#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include "Board/Board.hpp"
#include "Display_Board/Display_Board.hpp"
#include "Player/Player.hpp"
#include "Tile/InitTiles.hpp"
#include "Tile/TileQueue.hpp"
#include <vector>
#include <random>
#include <string>

class Display_Board;


/**
 * @class Game
 * @brief Pilote la logique principale de la partie.
 *
 * Cette classe orchestre :
 *  - la création des joueurs et la gestion de leur ordre de jeu ;
 *  - l'initialisation du plateau et des bonus ;
 *  - le chargement et la pioche des tuiles ;
 *  - la boucle principale de tours / manches ;
 *  - les règles de placement, de capture de bonus, et les phases finales ;
 *  - le calcul et l'affichage du score final.
 *
 * Elle s’appuie sur :
 *  - Board pour représenter l’état du plateau,
 *  - Player pour les joueurs,
 *  - Tile / InitTiles / TileQueue pour les tuiles,
 *  - Display_Board pour l’affichage ASCII du plateau.
 */
class Game {
private:

    /// Liste des joueurs dans l’ordre de jeu.
    std::vector<Player> players;

    /// Liste des joueurs dans l’ordre de jeu.
    Board board;

    /// Objet chargé d’afficher le plateau (crée dynamiquement).
    Display_Board* display;

    /// Catalogue des tuiles chargées depuis un fichier JSON.
    InitTiles tileSet;

    /// File de pioche des tuiles (deck + fenêtre d’aperçu).
    TileQueue queue;

    /// Numéro de la manche en cours (1..maxRounds).
    int currentRound;

    /// Index du joueur courant dans le vecteur players.
    int currentPlayerIndex;

    /// Indique si la partie est terminée.
    bool gameOver;

public:

    /**
    * @brief Construit un objet Game dans son état initial.
    *
    * Ne lance pas automatiquement une partie, il faut appeler start().
    */
    Game();

    /**
    * @brief Destructeur.
    *
    * Libère notamment l’objet Display_Board alloué dynamiquement.
    */
    ~Game();

    /**
    * @brief Point d’entrée principal pour lancer une partie complète.
    *
    * Enchaîne :
    *  - setupPlayers()
    *  - setupBoard()
    *  - setupTiles()
    *  - placeStartingTiles()
    *  - runRounds()
    *  - finalSingleCellPhase()
    *  - finishAndScore()
    */
    void start();

    /**
    * @brief Demande à chaque joueur de placer sa case de départ.
    *
    * Chaque joueur choisit une position de spawn sur le plateau,
    * en respectant le fait que la case doit être libre.
    */
    void placeStartingTiles();


    /**
     * @brief Accès en lecture à la liste des joueurs.
     * @return Référence constante vers le vecteur de joueurs.
     */
    const std::vector<Player>& getPlayers() const { return players; }

    /**
    * @brief Convertit une couleur logique (red, blue, cyan…) en code ANSI.
    *
    * @param colorName Nom logique de couleur, éventuellement en français.
    * @return Séquence d’échappement ANSI prête à être envoyée sur la console.
    */
    std::string getAnsiColor(const std::string& colorName) const;

    /**
    * @brief Recherche un joueur par identifiant (version modifiable).
    * @param id Identifiant du joueur.
    * @return Référence vers le joueur correspondant.
    * @throw std::runtime_error si l'ID n'existe pas.
    */
    Player& getPlayerById(int id);


    /**
    * @brief Recherche un joueur par identifiant (version const).
    * @param id Identifiant du joueur.
    * @return Référence constante vers le joueur correspondant.
    * @throw std::runtime_error si l'ID n'existe pas.
    */
    const Player& getPlayerById(int id) const;

private:

    /**
    * @brief Configure les joueurs : nombre, noms, couleurs, ordre de jeu.
    *
    * Demande en entrée :
    *  - nombre de joueurs (2 à 9) ;
    *  - nom et couleur de chaque joueur parmi une liste disponible.
    *
    * Mélange ensuite l'ordre des joueurs aléatoirement.
    */
    void setupPlayers();

    /**
    * @brief Initialise le plateau en fonction du nombre de joueurs.
    *
    * Crée un Board de la bonne taille, y place les bonus,
    * puis initialise l’objet d’affichage Display_Board.
    */
    void setupBoard();

    /**
    * @brief Charge les tuiles depuis le fichier JSON et initialise la pioche.
    *
    * Utilise InitTiles pour charger le catalogue depuis "Shapes.json",
    * puis remplit TileQueue (avec mélange).
    */
    void setupTiles();

    /**
    * @brief Lance la boucle principale de manches.
    *
    * À chaque manche :
    *  - chaque joueur joue un tour dans l’ordre ;
    *  - endRound() est appelée en fin de manche ;
    *  - la boucle s’arrête si gameOver passe à true.
    *
    * @param maxRounds Nombre maximum de manches (par défaut 9).
    */
    void runRounds(int maxRounds = 9);

    /**
    * @brief Exécute le tour complet d’un joueur :
    *  - usage forcé éventuel du Rock bonus ;
    *  - pioche d’une tuile ;
    *  - séquence d’actions (rotate, flip, exchange, place, cancel).
    *
    * @param player Référence vers le joueur dont c’est le tour.
    */
    void playTurn(Player& player);

    /**
    * @brief Fin de manche : évalue si la partie doit s’arrêter.
    */
    void endRound();

    /**
    * @brief Indique si les conditions de fin de partie sont réunies.
    *
    * Pour l’instant, toujours false (à étendre selon les règles).
    */
    bool isGameOver() const;

    /**
    * @brief Affiche la tuile courante et la fenêtre des suivantes.
    *
    * Utilise TileQueue::display pour rendre une vue ASCII des tuiles.
    *
    * @param current Tuile actuellement en main.
    */
    void showQueueWithCurrent(const Tile& current) const;


    /**
    * @brief Propose au joueur d’échanger sa tuile avec l’une des suivantes.
    *
    * Utilise la fenêtre de TileQueue (1 à 5) et consomme un ticket
    * d’échange si l’opération réussit.
    *
    * @param current Référence sur la tuile courante (mise à jour en cas d’échange).
    * @return true si l’échange a eu lieu, false sinon.
    */
    bool promptExchange(Tile& current) ;


    /**
    * @brief Dialogue avec le joueur pour placer la tuile courante sur le plateau.
    *
    * Demande une origine sous forme "A0", "C12", etc.,
    * calcule l’empreinte de la tuile et vérifie les règles de placement.
    *
    * @param current Tuile courante (après rotations/flips éventuels).
    * @param playerId Identifiant du joueur qui place la tuile.
    * @return true si le placement est validé, false sinon.
    */
    bool promptPlace(Tile& current, int playerId);

    /**
    * @brief Supprime les espaces en début et fin de chaîne.
    *
    * @param s Chaîne source.
    * @return Chaîne nettoyée.
    */
    static std::string trim(const std::string& s);

    /**
    * @brief Parse un entier strictement (avec validation complète).
    *
    * @param s Chaîne à convertir.
    * @param out Référence de sortie pour la valeur.
    * @return true si la conversion a réussi, false sinon.
    */
    static bool parseIntStrict(const std::string& s, int& out);

    /**
    * @brief Lit un entier dans un intervalle [minVal, maxVal], avec validation stricte.
    *
    * @param prompt Message d’invite.
    * @param minVal Borne minimale.
    * @param maxVal Borne maximale.
    * @return L’entier saisi et validé.
    */
    static int readIntInRangeStrict(const std::string& prompt, int minVal, int maxVal);

    /**
    * @brief Lit une coordonnée colonne/ligne de type "A0", "C12", "AA7".
    *
    * Sépare les lettres (colonne) des chiffres (ligne), vérifie qu’ils sont dans
    * les bornes [0..maxCol-1] et [0..maxRow-1].
    *
    * @param prompt Message d’invite.
    * @param maxCol Nombre de colonnes du plateau.
    * @param maxRow Nombre de lignes du plateau.
    * @param outCol Sortie : indice de colonne.
    * @param outRow Sortie : indice de ligne.
    * @return true si la saisie est valide, false sinon.
    */
    static bool readColRow(const std::string& prompt, int maxCol, int maxRow, int& outCol, int& outRow);


    /**
    * @brief Variante simple pour lire un entier borné.
    */
    static int readIntInRange(const std::string& prompt, int minVal, int maxVal);

    /**
    * @brief Lit une réponse oui/non (y/n, o/n).
    * @return true pour oui, false pour non.
    */
    static bool readYesNo(const std::string& prompt);

    /**
    * @brief Lit un caractère appartenant à un ensemble autorisé.
    *
    * @param prompt Message affiché.
    * @param allowed Liste de caractères acceptés.
    * @return Un caractère parmi ceux autorisés.
    */
    static char readChoice(const std::string& prompt, const std::string& allowed);


    /**
    * @brief Vérifie si une empreinte de tuile peut être posée sur le plateau.
    *
    * Règles :
    *  - aucune cellule ne doit sortir du plateau ;
    *  - aucune cellule ne doit recouvrir une case occupée ;
    *  - l’empreinte doit toucher au moins une case du joueur ;
    *  - elle ne doit pas être adjacente à une case d’un autre joueur.
    *
    * @param pts Liste de coordonnées (x,y) de l’empreinte.
    * @param playerId Identifiant du joueur.
    * @return true si le placement est autorisé, false sinon.
    */
    bool canPlaceFootprint(const std::vector<std::pair<int,int>>& pts, int playerId) const;

    /**
    * @brief Applique effectivement l’empreinte sur le plateau
    *        et déclenche la détection de capture de bonus.
    *
    * @param pts Liste de coordonnées de la tuile posée.
    * @param playerId Identifiant du joueur.
    */
    void placeFootprint(const std::vector<std::pair<int,int>>& pts, int playerId);

    /**
    * @brief Gère l’utilisation obligatoire du Rock bonus.
    *
    * Demande à quel endroit placer une pierre 1x1 'X' sur une case vide.
    *
    * @param player Joueur possédant le Rock bonus.
    * @return true si la pierre a été placée, false sinon.
    */
    bool useRockBonus(Player& player);

    /**
    * @brief Affiche le plateau via Display_Board.
    */
    void displayBoard() const;


    /**
    * @brief Affiche l’ordre final des joueurs après le mélange initial.
    */
    void announceOrder() const;


    /**
    * @brief Structure de données pour représenter le score final d’un joueur.
    */
    struct FinalScore {
    int playerId;
    int maxSquare;
    int cellCount;
    };

    /**
    * @brief Phase finale : chaque joueur peut acheter une case 1x1 avec un coupon.
    *
    * Coût : 1 coupon d’échange.
    * Règles de placement identiques aux autres tuiles (contact + pas d’adjacence ennemie).
    */
    void finalSingleCellPhase();

    /**
    * @brief Dialogue pour placer une tuile 1x1 pour un joueur donné.
    *
    * @param playerId Identifiant du joueur.
    * @return true si le placement a réussi, false sinon.
    */
    bool promptPlaceSingleCell(int playerId);


    /**
    * @brief Termine la partie et lance le calcul et l’affichage des scores.
    */
    void finishAndScore();

    /**
    * @brief Calcule les scores de tous les joueurs.
    *
    * Pour chaque joueur :
    *  - compte le nombre de cases contrôlées ;
    *  - calcule la taille de son plus grand carré plein (DP).
    *
    * @return Vecteur de FinalScore (un par joueur).
    */
    std::vector<FinalScore> computeScores() const;


    /**
    * @brief Affiche les scores finaux triés et annonce le vainqueur.
    *
    * @param scores Liste des scores calculés par computeScores().
    */
    void printScores(const std::vector<FinalScore>& scores) const;
};



#endif // GAME_HPP_INCLUDED