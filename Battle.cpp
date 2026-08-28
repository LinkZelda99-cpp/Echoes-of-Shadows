#include "Battle.h"
#include "Dialogue.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

extern int loops;
BattleOutcome lastBattleOutcome = BattleOutcome::None;
int lastBattleEnemyId = -1;

int randint(int min, int max) {
    return min + rand() % (max - min + 1);
}

void battle(Player& player, int id) {
    switch (id) {
    case 1: {
        std::cout << "\n------ BATTLE ------\n";
        std::cout << "The Goblin attacks you.\n";

        int goblin_hp = 50;

        while (goblin_hp > 0) {
            std::cout << "Goblin HP: " << goblin_hp << std::endl;
            std::cout << "What would you like to do?\n";
            std::cout << "1. Attack\n";
            std::cout << "2. Run\n";
            std::cout << "Choice: ";

            int choice;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid choice.\n";
                continue;
            }

            switch (choice) {
            case 1: {
                std::cout << "\nYou attacked the Goblin.\nThe Goblin attacks you back!\n";

                int i = randint(1, 5);
                switch (i) {
                case 1: std::cout << "You lose 10 HP.\n"; player.health -= 10; break;
                case 2: std::cout << "OOF, you lose 30 HP!\n"; player.health -= 30; break;
                case 3: std::cout << "The Goblin misses!\n"; break;
                case 4: std::cout << "A weak hit! You lose 5 HP\n"; player.health -= 5; break;
                case 5: std::cout << "The Goblin misses!\n"; break;
                }

                goblin_hp -= 20;

                if (player.health <= 0) {
                    std::cout << "You have been slain by the Goblin. Game Over.\n";
                    // record the outcome
                    lastBattleOutcome = BattleOutcome::PlayerDefeated;
                    lastBattleEnemyId = id;
                    loops += 1;
                    player.health = 100;
                    player.gold = 0;
                    std::cout << "\n--------------------\n";
                    std::cout << "\n\033[3mYou wake back up... but something feels different.\033[0m\n";
                    std::cout << "\n--------------------\n";
                    return;
                }

                if (goblin_hp <= 0) {
                    // record the outcome
                    lastBattleOutcome = BattleOutcome::Victory;
                    lastBattleEnemyId = id;
                    player.gold += 10;
                    return;
                }
            }
                  break;

            case 2:
                lastBattleOutcome = BattleOutcome::Ran;
                lastBattleEnemyId = id;
                std::cout << "\nYou ran away! But the Goblin got you before you could get away.\n";
                player.health -= 10;
                return;

            default:
                std::cout << "Invalid choice.\n";
                break;
            }
        }
        break;
    }
    case 2: {
        std::cout << "\n------ BATTLE ------\n";
        std::cout << "The Skeleton attacks you.\n";
        int skeleton_hp = 70;

        while (skeleton_hp > 0) {
            std::cout << "Skeleton HP: " << skeleton_hp << std::endl;
            std::cout << "What would you like to do?\n";
            std::cout << "1. Attack\n";
            std::cout << "2. Run\n";
            std::cout << "Choice: ";

            int choice;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid choice.\n";
                continue;
            }

            switch (choice) {
            case 1: {
                std::cout << "\nYou attacked the Skeleton.\nThe Skeleton attacks you back!\n";

                int i = randint(1, 5);
                switch (i) {
                case 1: std::cout << "You lose 10 HP.\n"; player.health -= 10; break;
                case 2: std::cout << "OOF, you lose 30 HP!\n"; player.health -= 30; break;
                case 3: std::cout << "The Skeleton misses!\n"; break;
                case 4: std::cout << "A weak hit! You lose 5 HP\n"; player.health -= 5; break;
                case 5: std::cout << "The Skeleton misses!\n"; break;
                }

                skeleton_hp -= 20;

                if (player.health <= 0) {
                    std::cout << "You have been slain by the Skeleton. Game Over.\n";
                    lastBattleOutcome = BattleOutcome::PlayerDefeated;
                    lastBattleEnemyId = id;
                    loops += 1;
                    player.health = 100;
                    player.gold = 0;
                    std::cout << "\n--------------------\n";
                    std::cout << "\n\033[3mYou wake back up... but something feels different.\033[0m\n";
                    std::cout << "\n--------------------\n";
                    return;
                }
                if (skeleton_hp <= 0) {
                    lastBattleOutcome = BattleOutcome::Victory;
                    lastBattleEnemyId = id;
                    player.gold += 10;
                    return;
                }
            }
                  break;

            case 2:
                lastBattleOutcome = BattleOutcome::Ran;
                lastBattleEnemyId = id;
                std::cout << "\nYou ran away! But the Skeleton got you before you could get away.\n";
                player.health -= 10;
                return;

            default:
                std::cout << "Invalid choice.\n";
                break;
            }
        }
        break;
    }
    }
}
