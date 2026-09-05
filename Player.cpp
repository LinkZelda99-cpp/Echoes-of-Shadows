#include "Player.h"
#include <iostream>
#include "Dialogue.h"

Player::Player() {
    inventory.reserve(INVENTORY_SIZE);
}

void Player::showStats() const
{

    std::cout << "\n------ STATS ------\n";
    std::cout << "Health: " << health << "\n";
    std::cout << "Gold: " << gold << "\n";
    std::cout << "-------------------\n";

}

void Player::addItem(const Item& item) {
    if (static_cast<int>(inventory.size()) >= INVENTORY_SIZE) {
        std::cout << "Inventory full. Could not add " << item.name << ".\n";
        return;
    }
    inventory.push_back(item);
    std::cout << "Added '" << item.name << "' to inventory (slot " << (inventory.size() - 1) << ").\n";
}

bool Player::useItem(int index) {
    if (index < 0 || index >= static_cast<int>(inventory.size())) {
        std::cout << "Invalid inventory slot.\n";
        return false;
    }
    Item item = inventory[index];
    std::string result = item.use(*this);
    std::cout << result << "\n";
    if (item.consumable) {
        inventory.erase(inventory.begin() + index);
        return true;
    }
    return false;
}

void Player::dropItem(int index) {
    if (index < 0 || index >= static_cast<int>(inventory.size())) {
        std::cout << "Invalid inventory slot.\n";
        return;
    }
    std::cout << "Dropped '" << inventory[index].name << "'.\n";
    inventory.erase(inventory.begin() + index);
}

void Player::viewItem(int index) const {
    if (index < 0 || index >= static_cast<int>(inventory.size())) {
        std::cout << "Invalid inventory slot.\n";
        return;
    }
    const Item& it = inventory[index];
    std::cout << "\n" << it.name << "\n";
    std::cout << it.description << "\n";
}

void Player::showInventory() const {
    std::cout << "\n-- Inventory --\n";
    for (int i = 0; i < INVENTORY_SIZE; ++i) {
        std::cout << "Slot " << i << ": ";
        if (i < static_cast<int>(inventory.size())) {
            std::cout << inventory[i].name << "\n";
        } else {
            std::cout << "(empty)\n";
        }
    }
    std::cout << "---------------\n";
}

// Implement Item::use here to modi
// Player as needed.
std::string Item::use(Player& player) const {
    if (id == 100) { // Health Potion
        if (player.health >= 100) return "You are already at full health.";
        int heal = 20;
        player.health += heal;
        if (player.health > 100) player.health = 100;
        return "You drink the " + name + ". You recover " + std::to_string(heal) + " HP.";
    }
    if (id == 200) { // Old Sword
        return "You brandish the " + name + ". It looks worn but usable.";
    }
    return "You can't use that right now.";
}
