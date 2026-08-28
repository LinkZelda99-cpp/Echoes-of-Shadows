#pragma once

#include <string>

struct SaveData {
    std::string playerName;
    int loops;
};

bool saveGame(const SaveData& data);
bool loadGame(SaveData& data);