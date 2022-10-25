#include "deck.h"
#include "card.h"
#include <stdexcept>

Deck::Deck(bool includeJokers) {
    cardCount = (includeJokers ? 54 : 52);
    Deck::reset();
    // random shuffle by default
    Deck::shuffle();
}

Deck::~Deck() {
    for (auto ptr : remainingCards) {
        delete ptr;
    }
    for (auto ptr : dealedCards) {
        delete ptr;
    }
}

int Deck::getNumRemainingCards() const {
    return remainingCards.size();
}

void Deck::shuffle() {
    auto rng = std::default_random_engine {};
    std::shuffle(remainingCards.begin(), remainingCards.end(), rng);
}

Card* Deck::dealCards() {
    // Check if cards are available, then deal the last card
    if (remainingCards.size()) {
        auto card = remainingCards.back();
        dealedCards.push_back(card);
        remainingCards.pop_back();
        return card;
    } else {
        return nullptr;
    }
}

std::vector<Card*> Deck::dealCards(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Negative Amount of Cards");
    }
    std::vector<Card*> res;
    for (int i = 0; i < amount; i++) {
        res.push_back(Deck::dealCards());
    }
    return res;
}

Card* Deck::dealGivenCards(Card::Suit suit, Card::Face face) {
    for (int i = 0; i < remainingCards.size(); i++) {
        auto card = remainingCards[i];
        if (*card == Card(suit, face)) {
            dealedCards.push_back(card);
            remainingCards.erase(remainingCards.begin() + i);
            return card;
        }
    }
    return nullptr;
}

std::vector<Card*> Deck::dealGivenCards(const std::vector<std::pair<Card::Suit,
Card::Face>>& cards) {
    std::vector<Card*> res;
    for (auto card : cards) {
        res.push_back(Deck::dealGivenCards(card.first, card.second));
    }
    return res;
}

void Deck::reset() {
    // clean up existing garbage
    for (auto ptr : remainingCards) {
        delete ptr;
    }
    for (auto ptr : dealedCards) {
        delete ptr;
    }
    remainingCards.clear();
    dealedCards.clear();
    // generate a new deck
    const Card::Suit suitArr[] = {
        Card::heart, Card::diamond, Card::spade, Card::club
    };
    for (auto suit : suitArr) {
        for (int face = 1; face <= 13; face++) {
            remainingCards.push_back(
                new Card(suit, static_cast<Card::Face>(face)));
        }
    }
    if (includeJokers) {
        remainingCards.push_back(new Card(Card::nil, Card::black_joker));
        remainingCards.push_back(new Card(Card::nil, Card::red_joker));
    }
}
