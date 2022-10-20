#include <iostream>
#include <unordered_map>
#include <string>

#include "./card.h"

Card::Card(Card::Face face, Card::Suit suit)
:face(face), suit(suit) {}

Card::Card(Card::Face face, Card::Suit suit, std::string& owner)
:face(face), suit(suit), owner(owner) {}

Card::Card(const Card& card)
:face(card.face), suit(card.suit), owner(card.owner) {}

Card::Suit Card::getSuit() const {
    return suit;
}

Card::Face Card::getFace() const {
    return face;
}

bool Card::isOwned() const {
    return (owner != "");
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
