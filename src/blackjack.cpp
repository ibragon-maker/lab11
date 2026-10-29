#include "blackjack.h"
#include <iostream>
#include <random>
#include <string>
#include "card.h"
#include "deck.h"
#include "hand.h" // Make sure hand.h is included!

void play_blackjack() {
    int bankroll = 100;
    const int bet = 10;
    
    std::random_device device;
    std::mt19937 gen(device());

    Deck deck;
    deck.shuffle(gen);

    std::cout << "\n--- Arcade Blackjack ---\n";

    char play_again = 'y';
    while ((play_again == 'y' || play_again == 'Y') && bankroll > 0) {
        if (deck.remaining() < 15) { // Use remaining() instead of count()
            std::cout << "\n[Notice] Fewer than 15 cards remain. Reshuffling deck...\n";
            deck.shuffle(gen);
        }

        std::cout << "\n---------------------------------\n";
        std::cout << "Bankroll: $" << bankroll << " | Current Bet: $" << bet << "\n";

        Hand player_hand;
        Hand dealer_hand;

        // Initial deal (alternating 2 cards each)
        player_hand += deck.deal();
        dealer_hand += deck.deal();
        player_hand += deck.deal();
        dealer_hand += deck.deal();

        std::cout << "Dealer shows: " << dealer_hand[0] << " [Hidden Card]\n";
        std::cout << "Your hand:    " << player_hand << " (Value: " << player_hand.value() << ")\n";

        bool player_blackjack = player_hand.is_blackjack();
        bool dealer_blackjack = dealer_hand.is_blackjack();

        if (player_blackjack || dealer_blackjack) {
            std::cout << "\nDealer's hand: " << dealer_hand << " (Value: " << dealer_hand.value() << ")\n";
            if (player_blackjack && dealer_blackjack) {
                std::cout << "Both have Blackjack! It's a push.\n";
            } else if (player_blackjack) {
                int winnings = static_cast<int>(bet * 1.5);
                std::cout << "Blackjack! You win 3:2 ($" << winnings << ").\n";
                bankroll += winnings;
            } else {
                std::cout << "Dealer has Blackjack. You lose $" << bet << ".\n";
                bankroll -= bet;
            }
        } else {
            // Player turn
            char choice = ' ';
            while (player_hand.value() < 21) {
                std::cout << "Hit or stand? (h/s): ";
                std::cin >> choice;

                if (choice == 'h' || choice == 'H') {
                    player_hand += deck.deal();
                    std::cout << "Your hand: " << player_hand << " (Value: " << player_hand.value() << ")\n";
                    if (player_hand.is_bust()) {
                        std::cout << "Bust! You went over 21.\n";
                        break;
                    }
                } else if (choice == 's' || choice == 'S') {
                    break;
                }
            }

            if (player_hand.is_bust()) {
                std::cout << "Dealer wins. You lose $" << bet << ".\n";
                bankroll -= bet;
            } else {
                // Dealer turn (draws until 17 or more; soft 17 stands)
                std::cout << "\nDealer's turn...\n";
                std::cout << "Dealer's hand: " << dealer_hand << " (Value: " << dealer_hand.value() << ")\n";

                while (dealer_hand.value() < 17) {
                    dealer_hand += deck.deal();
                    std::cout << "Dealer draws: " << dealer_hand << " (Value: " << dealer_hand.value() << ")\n";
                }

                int p_val = player_hand.value();
                int d_val = dealer_hand.value();

                std::cout << "\n--- Round Results ---\n";
                std::cout << "Your final value:     " << p_val << "\n";
                std::cout << "Dealer's final value: " << d_val << "\n";

                if (dealer_hand.is_bust()) {
                    std::cout << "Dealer busts! You win $" << bet << ".\n";
                    bankroll += bet;
                } else if (p_val > d_val) {
                    std::cout << "You win $" << bet << "!\n";
                    bankroll += bet;
                } else if (p_val < d_val) {
                    std::cout << "Dealer wins. You lose $" << bet << ".\n";
                    bankroll -= bet;
                } else {
                    std::cout << "Push! Your bet is returned.\n";
                }
            }
        }

        if (bankroll <= 0) {
            std::cout << "\nYour bankroll has reached $0. Game Over!\n";
            break;
        }

        std::cout << "\nPlay another round? (y/n): ";
        std::cin >> play_again;
    }

    std::cout << "\nThanks for playing! Final Bankroll: $" << bankroll << "\n";
}
