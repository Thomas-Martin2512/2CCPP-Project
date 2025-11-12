#ifndef TILEQUEUE_HPP_INCLUDED
#define TILEQUEUE_HPP_INCLUDED

#include <deque>
#include <random>
#include <vector>
#include <iostream>
#include "Tile/Tile.hpp"
#include "Tile/InitTiles.hpp"

/**
 * @class TileQueue
 * @brief Gère une pioche (deck) de tuiles avec aperçu et échange.
 *
 * Responsabilités :
 * - Initialiser la pioche depuis un catalogue (`InitTiles`) ;
 * - Mélanger (avec graine optionnelle) et reconstituer la pioche à l’épuisement ;
 * - Tirer la tuile courante, prévisualiser les prochaines et échanger via une « fenêtre » ;
 * - Afficher une représentation ASCII des tuiles.
 *
 * Remarques :
 * - La classe maintient deux structures : `all_` (toutes les tuiles) et `deck_` (ordre courant).
 * - Si `reshuffleOnExhaustion_` est vrai, la pioche se reconstitue automatiquement à vide.
 * - Non thread-safe.
 */
class TileQueue {
public:
    /**
    * @brief Construit une file de tuiles.
    * @param reshuffleOnExhaustion Si vrai, la pioche est reconstituée automatiquement à l’épuisement.
    */
    explicit TileQueue(bool reshuffleOnExhaustion = true);


    /**
     * @brief Initialise la pioche à partir d’un catalogue de tuiles.
     *
     * Copie les tuiles de `src` dans `all_`, puis (optionnellement) mélange et remplit `deck_`.
     *
     * @param src Source des tuiles (catalogue).
     * @param shuffle Si vrai, mélange l’ordre initial.
     * @param seed Graine RNG utilisée pour le mélange (par défaut, issue de `std::random_device`).
     * @return true si l’initialisation a réussi, false si `src` est vide.
     */
    bool initFrom(const InitTiles& src, bool shuffle = true, std::uint32_t seed = std::random_device{}());

    /**
     * @brief Prévisualise les prochaines tuiles sans les retirer.
     * @param count Nombre de tuiles à prévisualiser (tronqué si la pioche est plus petite).
     * @return Copie des tuiles demandées en tête de `deck_`.
     */
    std::vector<Tile> peek(std::size_t count = 5) const;

    /**
     * @brief Tire (retire) la tuile en tête de la pioche.
     *
     * Si la pioche devient vide :
     * - si `reshuffleOnExhaustion_` est vrai, elle est reconstituée et mélangée ;
     * - sinon, lève une exception `std::runtime_error`.
     *
     * @return La tuile tirée.
     * @throws std::runtime_error si la pioche est vide et la reconstitution désactivée.
     */
    Tile draw();

    /**
     * @brief Échange la tuile courante avec une tuile de la « fenêtre » des prochaines.
     *
     * La fenêtre est limitée par `index` (< 5) et la taille actuelle de la pioche.
     * Trouve la tuile d’identifiant `currentId` dans `all_` et remplace la tuile à `index`
     * par celle-ci, en retournant l’ancienne tuile de la fenêtre (nouvelle tuile courante).
     *
     * @param index Position (0..4) dans la fenêtre des prochaines tuiles.
     * @param currentId Identifiant de la tuile courante à remettre dans la fenêtre.
     * @return La nouvelle tuile courante si l’échange est possible, sinon `std::nullopt`.
     */
    std::optional<Tile> exchangeWithWindow(std::size_t index, const std::string& currentId);

    /**
     * @brief Affiche la tuile courante et un aperçu des suivantes en ASCII.
     * @param os Flux de sortie (ex. `std::cout`).
     * @param current Tuile courante à afficher à gauche.
     * @param nextCount Nombre de tuiles à afficher dans la fenêtre (à droite).
     */
    void display(std::ostream& os, const Tile& current, std::size_t nextCount = 5) const;

    /**
     * @brief Nombre de tuiles restantes dans la pioche.
     */
    std::size_t size() const { return deck_.size(); }

    /**
     * @brief Indique si la pioche est vide.
     */
    bool empty() const { return deck_.empty(); }

    /**
     * @brief Récupère les identifiants des prochaines tuiles (aperçu).
     * @param count Nombre d’identifiants souhaités.
     * @return Un vecteur d’IDs des prochaines tuiles.
     */
    std::vector<std::string> nextTileIds(std::size_t count = 5) const;

private:
    /** @brief Reconstituer/mélanger automatiquement la pioche lorsqu’elle est épuisée. */
    bool reshuffleOnExhaustion_;
    /** @brief Copie de toutes les tuiles (catalogue de référence). */
    std::vector<Tile> all_;
    /** @brief Pioche courante (ordre de tirage). */
    std::deque<Tile> deck_;
    /** @brief Générateur pseudo-aléatoire pour les mélanges. */
    mutable std::mt19937 rng_;

    /**
     * @brief Reconstitue `deck_` à partir de `all_` et mélange.
     */
    void reshuffle_();

    /**
     * @brief Construit une représentation ASCII d’une tuile.
     * @param t Tuile à représenter.
     * @param fill Caractère de remplissage (par défaut `\xDB`, bloc plein).
     * @return Vecteur de lignes ASCII.
     */
    static std::vector<std::string> asciiTile_(const Tile& t, char fill = '\xDB');

    /**
     * @brief Empile horizontalement plusieurs blocs ASCII avec un espacement.
     * @param blocks Liste de tuiles ASCII (chaque bloc est un tableau de lignes).
     * @param gap Nombre d’espaces à insérer entre les blocs.
     * @return Lignes ASCII composées.
     */
    static std::vector<std::string> hstack_(const std::vector<std::vector<std::string>>& blocks, int gap = 14);

    /**
    * @brief Recherche une tuile par identifiant dans `all_`.
    * @param id Identifiant recherché.
    * @return La tuile trouvée, sinon `std::nullopt`.
    */
    std::optional<Tile> findInAll_(const std::string& id) const;
};

#endif // TILEQUEUE_HPP_INCLUDED