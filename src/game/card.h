#ifndef CARD_H
#define CARD_H

class Card {
public:
    enum Suit {
        heart = 0,
        diamond = 1,
        spade = 2,
        club = 3
    };

     // TODO: not sure if needed
    enum Face {
        ace = 1,
        two = 2,
        three = 3,
        four = 4,
        five = 5,
        six = 6,
        seven = 7,
        eight = 8,
        nine = 9,
        ten = 10,
        jack = 11,
        queen = 12,
        king = 13,
        black_joker = 14,
        red_joker = 15
    };

private:
    Suit suit;
    Face face;
    // the player index who owns the card, -1 means not owned by anyone
    int owner;
public:
    Card ();
    Card (Face face, Suit suit);
    Card (Face face, Suit suit, int owner);
    Card (const Card& card);
    ~Card ();
    Suit getSuit() const;
    int getFace() const;
    bool isOwned() const;
    int getOwner() const;
    void display() const;
    bool comparator(const Card& card1, const Card& card2) const;
};

#endif