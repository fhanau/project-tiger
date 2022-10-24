#include <iostream>
#include <unordered_map>
#include <string>

#include "./card.h"

Card::Card(Card::Suit suit, Card::Face face)
:suit(suit), face(face) {}

Card::Card(Card::Suit suit, Card::Face face, std::string& owner)
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

void Card::display() const {
    // may change output stream if needed
    std::unordered_map<int, std::string> faceName = {
        {ace, "A"}, {jack, "J"}, {queen, "Q"},
        {king, "K"}, {black_joker, "BJ"}, {red_joker, "RJ"}
    };
    std::cout << suit << "-";
    if (faceName.find(face) != faceName.end()) {
        std::cout << faceName[face];
    } else {
        std::cout << face;
    }
    std::cout << std::endl;
}

bool Card::comparator(const Card& card1, const Card& card2) const {
    // TODO(Songheng): implement
    return true;
}

bool Card::operator==(const Card& rhs) const {
    return typeid(*this) == typeid(rhs) 
        && this->getFace() == rhs.getFace()
        && this->getSuit() == rhs.getSuit();
}