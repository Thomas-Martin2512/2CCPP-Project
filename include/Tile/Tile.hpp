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

        /// Retourne l'identifiant de la tuile
        virtual const std::string& getId() const;
        virtual const std::string& getColor() const;
        virtual const std::vector<std::pair<int, int>>& getCells() const;

        virtual void setId(std::string& newId);
        virtual void setColor(std::string& newColor);

        virtual void rotate();
        virtual void flip();
        virtual void normalizeToOrigin();

        virtual std::pair<int, int> boxLimit() const;
        virtual std::vector<std::pair<int, int>>
        footprint(int originX, int originY, int rotations = 0, bool flipped = false) const;

        virtual bool shapeEquals(const Tile& other) const;

    private:
        static void normalize(std::vector<std::pair<int, int>>& pts);
};

#endif // TILE_HPP_INCLUDED