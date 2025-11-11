#include "../../include/Player/Player.hpp"
#include <iostream>



int Player::nextID = 1;
Player::Player() : id(nextID++) {}

Player::Player(const std::string& playerName, const std::string& playerColor)
    : id(nextID++), name(playerName), color(playerColor) {}



void Player::inputPlayerInfo(int playerNumber) {
    std::cout << "=== Player " << playerNumber << " ===\n";
    std::cout << "Enter a player name : ";
    std::getline(std::cin,name);
    std::cout << "Enter a valid color : ";
    std::getline(std::cin,color);
    std::cout << "Player " << name << " saved !" << std::endl;
}

void Player::displayInfo() const {
    std::cout << "ID: " << id
              << "Name: " << name
              << "Color: " << color << std::endl;
}

int Player::getID() const {
    return id;
}

std :: string Player::getName() const {
    return name;
}

std :: string Player::getColor() const {
    return color;
}

void Player::setName(const std::string& newName) {
    this->name = newName;
}

void Player::setColor(const std :: string & newColor) {
    this->color = newColor;
}