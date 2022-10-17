#ifndef SRC_GAME_DECK_H_
#define SRC_GAME_DECK_H_

#include <random>
#include <utility>
#include <list>
#include <vector>

#include "./card.h"

class Deck {
 private:
    bool includeJokers;
    std::list<Card*> remainingCards;
    std::list<Card*> dealedCards;

 public:
    Deck();
    explicit Deck(bool includeJokers);
    ~Deck();
    void shuffle();
    void reset();
    // deal single/multiple cards
    Card* dealCards();
    Card* dealCards(int amount);
    Card* dealGivenCards(Card::Suit suit, Card::Face face);
    Card* dealGivenCards(const std::vector<std::pair<Card::Suit,
        Card::Face>>& cards);
};

#endif  // SRC_GAME_DECK_H_
