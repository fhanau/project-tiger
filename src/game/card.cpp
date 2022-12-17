#include <iostream>
#include <unordered_map>
#include <string>

#include "card.h"

Card::Card(Card::Suit suit, Card::Face face)
:suit(suit), face(face) {}

Card::Card(Card::Suit suit, Card::Face face, const std::string& owner)
:suit(suit), face(face), owner(owner) {}

Card::Card(const Card& card)
:suit(card.suit), face(card.face), owner(card.owner) {}

Card::~Card() {}

Card::Suit Card::getSuit() const {
    return suit;
}

Card::Face Card::getFace() const {
    return face;
}

bool Card::isOwned() const {
    return (owner != "");
}

void Card::setOwner(std::string owner) {
    this->owner = owner;
}

std::string Card::getOwner() const {
    if (!isOwned()) {
        throw -1;
    }
    return owner;
}

bool Card::operator==(const Card& rhs) const {
    return typeid(*this) == typeid(rhs)
        && this->getFace() == rhs.getFace()
        && this->getSuit() == rhs.getSuit();
}
