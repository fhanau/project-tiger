#include "gtest/gtest.h"

#include "card.h"

TEST(cardTest1, testGetter) {
    Card card(Card::heart, Card::king);
    EXPECT_EQ(card.getFace(), Card::king);
    EXPECT_EQ(card.getSuit(), Card::heart);
}

TEST(cardTest2, testOwner) {
    Card card(Card::heart, Card::king);
    EXPECT_FALSE(card.isOwned());
    card.setOwner("tiger1");
    EXPECT_TRUE(card.isOwned());
    EXPECT_EQ(card.getOwner(), "tiger1");
}

TEST(cardTest3, testOperatorOverloading) {
    Card card1(Card::heart, Card::king);
    Card card2(Card::heart, Card::king);
    Card card3(Card::club, Card::king);
    EXPECT_TRUE(card1 == card2);
    EXPECT_FALSE(card1 == card3);
}
