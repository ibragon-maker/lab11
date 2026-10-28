/**
 * @file hand.cpp
 * @brief Implementation of the Hand class for the Blackjack lab.
 */

#include "hand.h"

Hand::Hand() : m_count(0) {}

void Hand::clear() {
    m_count = 0;
}

Hand& Hand::operator+=(const Card& card) {
    if (m_count < 11) {
        m_cards[m_count++] = card;
    }
    return *this;
}

const Card& Hand::operator[](int index) const {
    return m_cards[index];
}

int Hand::count() const {
    return m_count;
}

int Hand::value() const {
    int total = 0;
    int ace_count = 0;

    for (int i = 0; i < m_count; ++i) {
        int r = m_cards[i].rank();
        
        // Assuming an Ace is represented by rank 1
        if (r == 1) { 
            ace_count++;
            total += 11;
        } else if (r >= 10) { 
            // Face cards (10, Jack, Queen, King)
            total += 10;
        } else {
            total += r;
        }
    }

    // Convert Aces from 11 to 1 if the total exceeds 21
    while (total > 21 && ace_count > 0) {
        total -= 10;
        ace_count--;
    }

    return total;
}

bool Hand::is_blackjack() const {
    return m_count == 2 && value() == 21;
}

bool Hand::is_bust() const {
    return value() > 21;
}

std::ostream& operator<<(std::ostream& os, const Hand& hand) {
    for (int i = 0; i < hand.count(); ++i) {
        os << hand[i];
        if (i < hand.count() - 1) {
            os << " ";
        }
    }
    return os;
}
