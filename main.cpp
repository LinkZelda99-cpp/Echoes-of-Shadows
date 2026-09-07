#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>

#include "World.h"
#include "Player.h"
#include "Dialogue.h"

// Global loop counter
int loops = 0;

static void printIntro(int /*loops*/, const std::string& name) {
    std::cout << "\nWelcome, " << name << "!\n";
    std::cout << "----------------------\n";

    typeText(
        "\033[3mThe shadows overwhelm your senses... darker, darker, yet darker.\033[0m\n",
        45
    );

    pauseFor(1200);

    typeText(
        "\033[3mThe darkness grows stronger... until suddenly, it vanishes.\033[0m\n",
        55
    );

    pauseFor(1800);

    typeText(
        "You find yourself somewhere you've never been before.\n",
        65
    );

    std::cout << "----------------------\n";

    waitForEnter();
}

static void printWorldDescription() {
    dialogue(
        "You are surrounded by strange things, as if you have entered another world. "
        "Ahead of you is a dark cave. To your right are the ruins of what was once a "
        "grand castle. To your left lies a forest that looks as though it has stood "
        "there for centuries.\n\n"
        "Where would you go in a place like this? Every path feels mysterious. "
        "Every direction feels like it hides something.\n"
    );
}

static void printLoopDialog(int currentLoops) {
    if (currentLoops == 0) {
        return;
    }

    if (currentLoops == 1) {
        return;
    }

    std::cout << "[MENU START LINE FOR LOOP "
              << currentLoops
              << " - WRITE HERE]\n\n";
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    Player player;

    std::cout << "Welcome to Echoes of Shadows.\n";
    std::cout << "What is your name? ";

    std::getline(std::cin, player.name);

    if (player.name.empty()) {
        player.name = "Player";
    }

    printIntro(loops, player.name);

    // The world description is shown when entering the world,
    // rather than every time the player returns from exploration.
    printWorldDescription();

    while (true) {
        printLoopDialog(loops);

        std::cout << "1. Explore\n";
        std::cout << "2. View stats\n";
        std::cout << "3. Inventory\n";
        std::cout << "4. Quit\n";

        int choice;

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout << "Invalid choice.\n";
            continue;
        }

        switch (choice) {

        case 1:
            explore(player);
            break;

        case 2:
            player.showStats();
            break;

        case 3: {
            while (true) {
                player.showInventory();

                std::cout
                    << "Enter slot number (0-"
                    << (Player::INVENTORY_SIZE - 1)
                    << ") to select an item, or 'e' to exit: ";

                std::string input;
                std::cin >> input;

                if (input == "e" ||
                    input == "E" ||
                    input == "exit") {
                    break;
                }

                int slot = -1;

                try {
                    slot = std::stoi(input);
                }
                catch (...) {
                    std::cout << "Invalid input.\n";
                    continue;
                }

                if (slot < 0 ||
                    slot >= Player::INVENTORY_SIZE) {
                    std::cout << "Invalid slot number.\n";
                    continue;
                }

                if (slot >= static_cast<int>(player.inventory.size())) {
                    std::cout << "That slot is empty.\n";
                    continue;
                }

                std::cout
                    << "Selected '"
                    << player.inventory[slot].name
                    << "' - choose action:\n";

                std::cout
                    << "1. Use\n"
                    << "2. View description\n"
                    << "3. Drop\n"
                    << "4. Exit\n";

                int action;

                if (!(std::cin >> action)) {
                    std::cin.clear();
                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout << "Invalid choice.\n";
                    continue;
                }

                if (action == 1) {
                    player.useItem(slot);
                }
                else if (action == 2) {
                    player.viewItem(slot);
                }
                else if (action == 3) {
                    player.dropItem(slot);
                }
                else if (action == 4) {
                    break;
                }
                else {
                    std::cout << "Invalid action.\n";
                }
            }

            break;
        }

        case 4:
            return 0;

        default:
            std::cout << "Invalid choice.\n";
            break;
        }
    }
}