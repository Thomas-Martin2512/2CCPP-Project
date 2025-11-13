#include "../../include/Player/Player.hpp"
#include <iostream>



/**
 * @brief Initialisation du compteur global d’IDs.
 */
int Player::nextID = 1;


/**
 * @brief Constructeur par défaut.
 *
 * Attribue un ID unique automatiquement.
 */
Player::Player() : id(nextID++) {}

/**
 * @brief Constructeur paramétré.
 *
 * @param playerName Nom du joueur.
 * @param playerColor Couleur logique.
 */
Player::Player(const std::string& playerName, const std::string& playerColor)
    : id(nextID++), name(playerName), color(playerColor) {}

/**
 * @brief Demande interactivement le nom et la couleur au joueur.
 */
void Player::inputPlayerInfo(int playerNumber) {
    std::cout << "=== Player " << playerNumber << " ===\n";
    std::cout << "Enter a player name : ";
    std::getline(std::cin,name);
    std::cout << "Enter a valid color : ";
    std::getline(std::cin,color);
    std::cout << "Player " << name << " saved !" << std::endl;
}

/**
 * @brief Affiche les informations du joueur (debug).
 */
void Player::displayInfo() const {
    std::cout << "ID: " << id
              << "Name: " << name
              << "Color: " << color << std::endl;
}

/** @return ID unique */
int Player::getID() const {
    return id;
}

/** @return nom du joueur */
std :: string Player::getName() const {
    return name;
}

/** @return couleur du joueur */
std :: string Player::getColor() const {
    return color;
}


/** @brief Change le nom du joueur */
void Player::setName(const std::string& newName) {
    this->name = newName;
}

/** @brief Change la couleur du joueur */
void Player::setColor(const std :: string & newColor) {
    this->color = newColor;
}