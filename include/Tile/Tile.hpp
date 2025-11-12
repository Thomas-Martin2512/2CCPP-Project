#ifndef TILE_HPP_INCLUDED
#define TILE_HPP_INCLUDED

#include <string>
#include <vector>
#include <utility>


/**
 * @class Tile
 * @brief Représente une tuile du jeu (forme, couleur, rotation, etc.)
 *
 * Chaque tuile contient un identifiant, une couleur et un ensemble de coordonnées (x, y)
 * représentant la forme dans une grille
*/
class Tile {
    protected:
        std::string id_;    ///< Identifiant unique de la tuile (ex: "TILE_00")
        std::string color_;     ///< Couleur de la tuile
        std::vector<std::pair<int, int>> cells_;    ///< Coordonnées des cases occupées

    public:
        Tile() = default;
        /**
        * @brief Construit une tuile.
        * @param id Identifiant unique
        * @param cells Coordonnées des cases formants la tuile
        * @Param color Couleur de la tuile qui va être posée
        */
        Tile(std::string id, std::vector<std::pair<int, int>> cells, std::string color = "green");
        virtual ~Tile() = default;

        /**
         * @brief Retourne l'identifiant de la tuile.
         * @return L'identifiant unique sous forme de chaîne.
         */
        virtual const std::string& getId() const;

        /**
        * @brief Retourne la couleur de la tuile.
        * @return La couleur sous forme de chaîne (ex. "red", "blue", "green").
        */
        virtual const std::string& getColor() const;

        /**
        * @brief Retourne les coordonnées des cases formant la tuile.
        * @return Un vecteur de paires (x, y) représentant les positions relatives.
        */
        virtual const std::vector<std::pair<int, int>>& getCells() const;

        /**
         * @brief Assigne un nouvel id à la tuile
         * @param newId nouvel identifiant
         */
        virtual void setId(std::string& newId);

        /**
         * @brief Assigne une nouvelle couleur à la tuile
         * @param newColor nouvelle couleur
         */
        virtual void setColor(std::string& newColor);

        /**
         * @brief Effectue une rotation de 90° vers la gauche sur la tuile.
         *
         * Modifie les coordonnées internes de la tuile afin de refléter
         * une rotation dans le sens antihoraire.
         */
        virtual void rotate();

        /**
         * @brief Applique un effet miroir horizontal à la tuile.
         *
         * Inverse la forme de la tuile selon l’axe vertical.
         */
        virtual void flip();


        /**
         * @brief Normalise la tuile par rapport à l’origine (0, 0).
         *
         * Translate les coordonnées pour que la tuile commence à (0, 0).
         * Utile pour la comparaison ou l’alignement.
         */
        virtual void normalizeToOrigin();

        /**
         * @brief Calcule les limites maximales de la tuile.
         * @return Une paire (largeur, hauteur) correspondant aux dimensions maximales du motif.
         */
        virtual std::pair<int, int> boxLimit() const;

        /**
        * @brief Retourne l’empreinte de la tuile sur le plateau.
        *
        * @param originX Coordonnée X de l’origine.
        * @param originY Coordonnée Y de l’origine.
        * @param rotations Nombre de rotations de 90° à appliquer (0 à 3).
        * @param flipped Si vrai, applique une symétrie miroir avant placement.
        * @return Un vecteur de coordonnées absolues représentant la position de chaque case.
        */
        virtual std::vector<std::pair<int, int>>
        footprint(int originX, int originY, int rotations = 0, bool flipped = false) const;

        /**
        * @brief Compare la forme de la tuile avec une autre.
        * @param other Autre tuile à comparer.
        * @return true si les formes sont identiques (même disposition de cases), false sinon.
        */
        virtual bool shapeEquals(const Tile& other) const;

    private:
        /**
        * @brief Normalise un ensemble de points par rapport à l’origine.
        * @param pts Vecteur de paires (x, y) à ajuster.
        *
        * Utilisée en interne pour repositionner les coordonnées
        * de manière à ce que le point le plus haut/gauche devienne (0, 0).
        */
        static void normalize(std::vector<std::pair<int, int>>& pts);
};

#endif // TILE_HPP_INCLUDED