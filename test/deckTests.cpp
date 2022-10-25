#include "gtest/gtest.h"

#include "deck.h"


TEST(deckTest1, testConstructor) {
    Deck deck(false);
    ASSERT_EQ(deck.getNumRemainingCards(), 52);
}
