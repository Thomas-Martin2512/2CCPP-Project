#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED
#include <string>



/**
 * @class Player
 * @brief Représente un joueur dans la partie.
 *
 * Cette classe stocke :
 *  - un identifiant unique (attribué automatiquement),
 *  - un nom,
 *  - une couleur (utilisée pour l’affichage ANSI),
 *  - un nombre de tickets d’échange,
 *  - deux types de bonus : Rock et Stealth.
 *
 * Le joueur ne possède aucune logique de jeu : il s'agit uniquement
 * d'un conteneur d'informations utilisé par Game et Board.
 */
class Player {
    private:
        /**
        * @brief Identifiant incrémental partagé entre tous les joueurs.
        *
        * Chaque nouveau joueur obtient un ID unique (1, 2, 3…).
        */
        static int nextID;

        /**
        * @brief Identifiant unique du joueur.
        */
        int id;

        /**
        * @brief Nom du joueur tel que défini au lancement du jeu.
        */
        std::string name;


        /**
        * @brief Couleur logique associée au joueur (red, blue, cyan…).
        *
        * Cette couleur est traduite en ANSI par Game pour l’affichage du plateau.
        */
        std::string color;

        /**
        * @brief Nombre de tickets d’échange disponibles.
        *
        * Le joueur commence avec 1 ticket par défaut.
        * Les tickets peuvent être gagnés en capturant un Exchange bonus.
        */
        int exchangeCoupons = 1;

        /**
        * @brief Indique si le joueur possède un Rock bonus disponible.
        *
        * Lorsqu'il est activé, le joueur doit placer une pierre 1x1
        * en début de tour.
        */
        bool rockBonusAvailable = false;

        /**
        * @brief Indique si le joueur possède un Stealth bonus.
        */
        bool stealthBonusAvailable = false;

    public:
        /**
        * @brief Constructeur par défaut.
        *
        * Assigne automatiquement un ID unique au joueur.
        */
        Player();

        /**
        * @brief Constructeur initialisant nom et couleur.
        * @param playerName Nom choisi par l'utilisateur.
        * @param playerColor Couleur logique ("red", "blue", etc.).
        */
        Player(const std::string& playerName, const std::string& playerColor);

        /**
        * @brief Demande à l'utilisateur de saisir nom et couleur (mode interactif).
        * @param playerNumber Numéro d'ordre du joueur à afficher.
        */
        void inputPlayerInfo(int playerNumber);

        /**
        * @brief Affiche les informations du joueur (debug).
        */
        void displayInfo() const;

        /** @return ID unique du joueur */
        int getID() const;

        /** @return Nom du joueur */
        std::string getName() const;

        /** @return Couleur du joueur */
        std::string getColor() const;

        /** @return true si le joueur possède un Rock bonus */
        bool hasRockBonus() const { return rockBonusAvailable; }

        /** @return true si le joueur possède un Stealth bonus */
        bool hasStealthBonus() const { return stealthBonusAvailable; }

        /** @brief Change le nom du joueur */
        void setName(const std::string& newName);

        /** @brief Change la couleur logique du joueur */
        void setColor(const std::string& newColor);

        /** @brief Active ou désactive le Rock bonus */
        void setRockBonusAvailable(bool v) { rockBonusAvailable = v; }

        /** @brief Active ou désactive le Stealth bonus */
        void setStealthBonusAvailable(bool v) { stealthBonusAvailable = v; }


        /** @return Nombre actuel de coupons d'échange */
        int getExchangeCoupons() const { return exchangeCoupons; }

        /** @brief Ajoute un coupon d’échange (capturé via bonus E) */
        void addExchangeCoupon() { exchangeCoupons++; }

        /** @brief Utilise un coupon d’échange si disponible */
        void useExchangeCoupon() { if (exchangeCoupons > 0) exchangeCoupons--; }
    };

#endif // PLAYER_HPP_INCLUDED
