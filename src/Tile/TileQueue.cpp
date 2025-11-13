/**
* @file TileQueue.cpp
 * @brief Implémentation de TileQueue — gestion d’une pioche de tuiles, aperçu, échange et affichage ASCII.
 */


#include "../../include/Tile/TileQueue.hpp"
#include <algorithm>
#include <stdexcept>



/**
 * @brief Construit la pioche.
 * @param reshuffleOnExhaustion Si vrai, reconstitue/mélange la pioche automatiquement à l’épuisement.
 */
TileQueue::TileQueue(bool reshuffleOnExhaustion)
  : reshuffleOnExhaustion_(reshuffleOnExhaustion), rng_(std::random_device{}()) {}


/**
 * @brief Initialise la pioche depuis un catalogue, avec mélange optionnel et graine donnée.
 *
 * Copie `src.all()` dans `all_`, initialise le RNG avec `seed`, optionnellement mélange
 * puis charge `deck_`.
 *
 * @param src Source des tuiles.
 * @param shuffle Si vrai, mélange l’ordre initial.
 * @param seed Graine de mélange (défaut: `std::random_device{ }()`).
 * @return true si au moins une tuile était disponible, false sinon.
 */
bool TileQueue::initFrom(const InitTiles& src, bool shuffle, std::uint32_t seed) {
    const auto& v = src.all();
    if (v.empty()) return false;
    all_ = v;
    rng_.seed(seed);
    std::vector<Tile> tmp = all_;
    if (shuffle) std::shuffle(tmp.begin(), tmp.end(), rng_);
    deck_.assign(tmp.begin(), tmp.end());
    return true;
}

/**
 * @brief Tire la tuile en tête de pioche.
 *
 * - Si `deck_` est vide et `reshuffleOnExhaustion_` est vrai, reconstitue et mélange avant de tirer.
 * - Si `deck_` est vide et `reshuffleOnExhaustion_` est faux, lance `std::runtime_error`.
 *
 * @return La tuile tirée.
 * @throws std::runtime_error Si la pioche est vide et la reconstitution désactivée.
 */
Tile TileQueue::draw() {
    if (deck_.empty()) {
        if (reshuffleOnExhaustion_) reshuffle_();
        else throw std::runtime_error("draw on empty deck");
    }
    Tile t = deck_.front();
    deck_.pop_front();
    if (deck_.empty() && reshuffleOnExhaustion_) reshuffle_();
    return t;
}

/**
 * @brief Prévisualise les prochaines tuiles sans les retirer.
 * @param count Nombre maximum de tuiles à renvoyer.
 * @return Copie des tuiles en tête de `deck_` (jusqu’à `count`).
 */
std::vector<Tile> TileQueue::peek(std::size_t count) const {
    std::vector<Tile> out;
    count = std::min<std::size_t>(count, deck_.size());
    out.reserve(count);
    for (std::size_t i = 0; i < count; ++i) out.push_back(deck_[i]);
    return out;
}

/**
 * @brief Reconstitue et mélange la pioche à partir de `all_`.
 */
void TileQueue::reshuffle_() {
    std::vector<Tile> tmp = all_;
    std::shuffle(tmp.begin(), tmp.end(), rng_);
    deck_.assign(tmp.begin(), tmp.end());
}

/**
 * @brief Recherche une tuile par identifiant dans le catalogue complet `all_`.
 * @param id Identifiant recherché.
 * @return La tuile correspondante si trouvée, sinon `std::nullopt`.
 */
std::optional<Tile> TileQueue::findInAll_(const std::string& id) const {
    for (const auto& t : all_) if (t.getId() == id) return t;
    return std::nullopt;
}

/**
 * @brief Retourne les identifiants des prochaines tuiles (aperçu).
 * @param count Nombre d’IDs souhaités.
 * @return Vecteur d’identifiants ordonnés comme dans `deck_`.
 */
std::vector<std::string> TileQueue::nextTileIds(std::size_t count) const {
    auto nxt = peek(count);
    std::vector<std::string> ids;
    ids.reserve(nxt.size());
    for (auto& t : nxt) ids.push_back(t.getId());
    return ids;
}

/**
 * @brief Échange la tuile courante avec une tuile de la fenêtre des prochaines.
 *
 * Contraintes :
 * - `index` doit être < 5 et < `deck_.size()` ;
 * - `currentId` doit exister dans `all_` ;
 * - Échange la tuile à `index` contre celle d’ID `currentId`, et retourne l’ancienne tuile de la fenêtre.
 *
 * @param index Index dans la fenêtre (0..4).
 * @param currentId ID de la tuile courante à replacer dans la fenêtre.
 * @return La nouvelle tuile courante si l’échange a eu lieu, sinon `std::nullopt`.
 */
std::optional<Tile> TileQueue::exchangeWithWindow(std::size_t index, const std::string& currentId) {
    if (index >= 5 || index >= deck_.size()) return std::nullopt;

    auto curOpt = findInAll_(currentId);
    if (!curOpt) return std::nullopt;

    Tile newCurrent = deck_[index];
    deck_[index] = *curOpt;
    return newCurrent;
}

/**
 * @brief Calcule la largeur/hauteur englobante d’un motif de cellules.
 * @param cells Points de la tuile.
 * @return (width, height) = (maxX+1, maxY+1).
 */
static std::pair<int,int> boxWH(const std::vector<std::pair<int,int>>& cells) {
    if (cells.empty()) return {0,0};
    int maxX = 0, maxY = 0;
    for (auto [x,y] : cells) { maxX = std::max(maxX, x); maxY = std::max(maxY, y); }
    return {maxX+1, maxY+1};
}

/**
 * @brief Construit un bloc ASCII représentant une tuile.
 * @param t Tuile source.
 * @param fill Caractère de remplissage (par défaut bloc plein).
 * @return Vecteur de lignes ASCII (ou "(vide)" si aucune cellule).
 */
std::vector<std::string> TileQueue::asciiTile_(const Tile& t, char fill) {
    const auto& cells = t.getCells();
    auto [w,h] = boxWH(cells);
    if (w==0 || h==0) return {"(vide)"};
    std::vector<std::string> g(h, std::string(w, ' '));
    for (auto [x,y] : cells) if (x>=0 && y>=0 && x<w && y<h) g[y][x] = fill;
    return g;
}

/**
 * @brief Concatène horizontalement plusieurs blocs ASCII avec un espacement fixe.
 * @param blocks Blocs ASCII (chaque bloc est une grille de lignes de même largeur).
 * @param gap Nombre d’espaces entre blocs.
 * @return Lignes composées du collage horizontal.
 */
std::vector<std::string> TileQueue::hstack_(const std::vector<std::vector<std::string>>& blocks, int gap) {
    std::size_t H = 0;
    for (auto& b : blocks) H = std::max(H, b.size());
    std::vector<std::string> out(H, "");
    std::string sep(gap, ' ');
    for (std::size_t i=0;i<blocks.size();++i) {
        const auto& b = blocks[i];
        std::size_t w = b.empty()?0:b[0].size();
        for (std::size_t r=0;r<H;++r) {
            if (r < b.size()) out[r] += b[r];
            else out[r] += std::string(w, ' ');
            if (i+1<blocks.size()) out[r] += sep;
        }
    }
    return out;
}

/**
 * @brief Affiche la tuile courante et un aperçu des prochaines sous forme ASCII.
 * @param os Flux de sortie.
 * @param current Tuile courante à afficher.
 * @param nextCount Nombre de tuiles à prévisualiser.
 */
void TileQueue::display(std::ostream& os, const Tile& current, std::size_t nextCount) const {
    os << "Current Tile :\n\n";
    auto cur = asciiTile_(current, '\xDB');
    for (auto& line : cur)
        os << "    " << line << "\n";

    os << "\nNext Tiles :\n\n";

    auto nxt = peek(nextCount);
    std::vector<std::vector<std::string>> blocks;
    blocks.reserve(nxt.size());
    for (auto& t : nxt)
        blocks.push_back(asciiTile_(t, '\xDB'));

    auto lines = hstack_(blocks, 14);
    for (auto& line : lines)
        os << "    " << line << "\n";

    os << "\n";
}