#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <random>
#include <list>
#include <vector>

class Deck {
private:
    bool includeJokers;
    std::list<Card*> remainingCards;
    std::list<Card*> dealedCards;

public:
    Deck();
    Deck(bool includeJokers);
    ~Deck();
    void shuffle();
    void reset();
    // deal single/multiple cards
    Card* dealCards();
    Card* dealCards(int amount);
    Card* dealGivenCards(Card::Suit suit, Card::Face face); 
    Card* dealGivenCards(std::vector<std::pair<Card::Suit, Card::Face>>& cards); 
};

#endif