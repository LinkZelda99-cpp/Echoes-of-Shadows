#pragma once
#include <string>
#include <vector>
#include "Item.h"

class Player {

public:

    std::string name;
    int health = 100;
    int gold = 0;

    static constexpr int INVENTORY_SIZE = 10;
    std::vector<Item> inventory; // holds items, max INVENTORY_SIZE

    // Add an item to inventory. If full, prints a message.
    void addItem(const Item& item);

    // Use item at index (0-based). Returns true if item was consumed and removed.
    bool useItem(int index);

    // Drop item at index.
    void dropItem(int index);

    // View item's description at index.
    void viewItem(int index) const;

    // Print current inventory to stdout.
    void showInventory() const;

    void showStats() const;

    Player();

};
