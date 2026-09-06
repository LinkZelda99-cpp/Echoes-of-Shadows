#include "../Save.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>

int main() {
    SaveData data{"TestPlayer", 11};
    if (!saveGame(data)) {
        std::cerr << "saveGame failed\n";
        return 1;
    }

    SaveData loaded{};
    if (!loadGame(loaded)) {
        std::cerr << "loadGame failed\n";
        return 1;
    }

    if (loaded.playerName != data.playerName || loaded.loops != data.loops) {
        std::cerr << "Loaded data does not match saved data\n";
        return 1;
    }

    std::cout << "Save round-trip test passed.\n";
    return 0;
}
